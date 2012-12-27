#include "..\..\..\..\Libraries\GacUI\Public\Source\GacUI.h"
#include <math.h>
#include <amp.h>

using namespace concurrency;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	// SetupWindowsDirect2DRenderer() is required for GuiDirect2DElement
	return SetupWindowsDirect2DRenderer();
}

struct Particle
{
	float				positionX;
	float				positionY;
	float				velocityX;
	float				velocityY;
	float				mass;
	bool				enabled;

	Particle()
		:positionX(0.0f)
		,positionY(0.0f)
		,velocityX(0.0f)
		,velocityY(0.0f)
		,mass(0.0f)
		,enabled(false)
	{
	}
};

class Direct2DWindow : public GuiWindow
{
protected:
	// constants
	static const int						MaxParticle			= 1000;
	static const int						NoGravityRadius		= 420;

	// gravity system
	Particle								particleBufferA[MaxParticle];
	Particle								particleBufferB[MaxParticle];
	array_view<Particle, 1>*				particleViewA;
	array_view<Particle, 1>*				particleViewB;

	Particle*								oldParticleBuffer;
	Particle*								newParticleBuffer;
	array_view<Particle, 1>*				oldParticleView;
	array_view<Particle, 1>*				newParticleView;

	// operation states
	bool									dragging;
	float									createX;
	float									createY;
	float									shootX;
	float									shootY;

	// rendering resources
	ComPtr<ID2D1SolidColorBrush>			brushNoGravity;
	ComPtr<ID2D1SolidColorBrush>			brushParticle;

	void InitializeAmpBuffer()
	{
		particleViewA=new array_view<Particle, 1>(MaxParticle, particleBufferA);
		particleViewB=new array_view<Particle, 1>(MaxParticle, particleBufferB);
		particleViewB->discard_data();

		oldParticleBuffer=particleBufferA;
		newParticleBuffer=particleBufferB;
		oldParticleView=particleViewA;
		newParticleView=particleViewB;
	}

	void FinalizeAmpBuffer()
	{
		delete particleViewA;
		delete particleViewB;
	}

	void Step()
	{
		array_view<Particle, 1>& from=*oldParticleView;
		array_view<Particle, 1>& to=*newParticleView;
		parallel_for_each(oldParticleView->extent, [=](index<1> i) restrict(amp)
		{
			Particle p=from[i];
			p.positionX+=p.velocityX;
			p.positionY+=p.velocityY;
			to[i]=p;
		});
		newParticleView->synchronize();
	}

	void Swap()
	{
		Particle* buffer=oldParticleBuffer;
		oldParticleBuffer=newParticleBuffer;
		newParticleBuffer=buffer;

		array_view<Particle, 1>* view=oldParticleView;
		oldParticleView=newParticleView;
		newParticleView=view;
	}

	void AddParticle(float x, float y, float vx, float vy)
	{
		for(int i=0;i<MaxParticle;i++)
		{
			if(!oldParticleBuffer[i].enabled)
			{
				Particle p;
				p.positionX=x;
				p.positionY=y;
				p.velocityX=vx;
				p.velocityY=vy;
				p.enabled=true;
				oldParticleBuffer[i]=p;
				break;
			}
		}
		oldParticleView->refresh();
	}

	void AddManyParticles(float x, float y, float vx, float vy, int count)
	{
		srand((unsigned)time(0));
		for(int i=0;i<MaxParticle && count>0;i++)
		{
			if(!oldParticleBuffer[i].enabled)
			{
				Particle p;
				p.positionX=x+(float)(rand()-RAND_MAX/2)/RAND_MAX;
				p.positionY=y+(float)(rand()-RAND_MAX/2)/RAND_MAX;
				p.velocityX=vx;
				p.velocityY=vy;
				p.enabled=true;
				oldParticleBuffer[i]=p;
				count--;
			}
		}
		oldParticleView->refresh();
	}

	// arguments.rt is ID2D1RenderTarget.
	void element_Rendering(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
	{
		Step();

		int w=arguments.bounds.Width();
		int h=arguments.bounds.Height();
		{
			arguments.rt->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f));
			int x=arguments.bounds.Left()+(w-NoGravityRadius*2)/2;
			int y=arguments.bounds.Left()+(h-NoGravityRadius*2)/2;
			arguments.rt->DrawEllipse(
				D2D1::Ellipse(
					D2D1::Point2F(
						(float)(x+NoGravityRadius),
						(float)(y+NoGravityRadius)
						),
					(float)NoGravityRadius,
					(float)NoGravityRadius),
				brushNoGravity.Obj(),
				3.0f
				);
		}

		for(int i=0;i<MaxParticle;i++)
		{
			if(newParticleBuffer[i].enabled)
			{
				float x=newParticleBuffer[i].positionX;
				float y=newParticleBuffer[i].positionY;
				arguments.rt->DrawEllipse(
					D2D1::Ellipse(D2D1::Point2F(x-2.0f, y-2.0f), 2.0f, 2.0f),
					brushParticle.Obj()
					);
			}
		}

		if(dragging)
		{
			arguments.rt->DrawLine(
				D2D1::Point2F(createX, createY),
				D2D1::Point2F(shootX, shootY),
				brushNoGravity.Obj()
				);
		}

		Swap();
	}

	// The render target is going to be destroyed, any binded resources should be released.
	void element_BeforeRenderTargetChanged(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
	{
		brushNoGravity=0;
		brushParticle=0;
	}

	// The new render target is prepared, any binded resources are allowed to recreate now.
	void element_AfterRenderTargetChanged(GuiGraphicsComposition* sender, GuiDirect2DElementEventArgs& arguments)
	{
		ID2D1SolidColorBrush* brush;
		{
			brush=0;
			arguments.rt->CreateSolidColorBrush(D2D1::ColorF(0.8f, 0.4f, 0.2f), D2D1::BrushProperties(), &brush);
			brushNoGravity=brush;
		}
		{
			brush=0;
			arguments.rt->CreateSolidColorBrush(D2D1::ColorF(0.2f, 0.7f, 0.4f), D2D1::BrushProperties(), &brush);
			brushParticle=brush;
		}
	}

	void window_LeftButtonDown(GuiGraphicsComposition* sender, GuiMouseEventArgs& arguments)
	{
		shootX=createX=(float)arguments.x;
		shootY=createY=(float)arguments.y;
		dragging=true;
	}

	void window_LeftButtonUp(GuiGraphicsComposition* sender, GuiMouseEventArgs& arguments)
	{
		dragging=false;
		float vx=(shootX-createX)/40;
		float vy=(shootY-createY)/40;
		if(arguments.ctrl)
		{
			AddManyParticles(createX, createY, vx, vy, 100);
		}
		else
		{
			AddParticle(createX, createY, vx, vy);
		}
	}

	void window_MouseMove(GuiGraphicsComposition* sender, GuiMouseEventArgs& arguments)
	{
		if(dragging)
		{
			shootX=(float)arguments.x;
			shootY=(float)arguments.y;
		}
	}
public:
	Direct2DWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		,particleViewA(0)
		,particleViewB(0)
		,oldParticleBuffer(0)
		,newParticleBuffer(0)
		,oldParticleView(0)
		,newParticleView(0)
		,dragging(false)
	{
		InitializeAmpBuffer();

		SetText(L"Particle Simulator");
		SetMinimizedBox(false);
		SetMaximizedBox(false);
		SetSizeBox(false);
		SetClientSize(Size(960, 960));
		MoveToScreenCenter();
		{
			GuiDirect2DElement* element=GuiDirect2DElement::Create();
			element->Rendering.AttachMethod(this, &Direct2DWindow::element_Rendering);
			element->BeforeRenderTargetChanged.AttachMethod(this, &Direct2DWindow::element_BeforeRenderTargetChanged);
			element->AfterRenderTargetChanged.AttachMethod(this, &Direct2DWindow::element_AfterRenderTargetChanged);

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			composition->SetOwnedElement(element);
			GetContainerComposition()->AddChild(composition);
		}

		this->GetEventReceiver()->leftButtonDown.AttachMethod(this, &Direct2DWindow::window_LeftButtonDown);
		this->GetEventReceiver()->leftButtonUp.AttachMethod(this, &Direct2DWindow::window_LeftButtonUp);
		this->GetEventReceiver()->mouseMove.AttachMethod(this, &Direct2DWindow::window_MouseMove);
	}

	~Direct2DWindow()
	{
		FinalizeAmpBuffer();
	}
};

void GuiMain()
{
	Direct2DWindow window;
	GetApplication()->Run(&window);
}