#include <JudasEngine.h>

#include "Assets/Scripts/TestManager.h"
#include "Assets/Scripts/Tests/PrimitiveTest.h"
#include "Assets/Scripts/Tests/ComputeShaderTest.h"
#include "Assets/Scripts/Tests/TextureTest.h"

class GameApplication : public Judas_Engine::Application
{
public:
	GameApplication()
	{
		TestManagerLayer* testManagerLayer = new TestManagerLayer();
		testManagerLayer->AddTestLayer("Primitive", std::make_shared<PrimitiveTestLayer>());
		testManagerLayer->AddTestLayer("Texture", std::make_shared<TextureLayer>());
		testManagerLayer->AddTestLayer("Compute Shader", std::make_shared<ComputeShaderLayer>());
		PushLayer(testManagerLayer);
	}
	~GameApplication()
	{

	}
};

Judas_Engine::Application* Judas_Engine::CreateApplication()
{
	return new GameApplication();
}