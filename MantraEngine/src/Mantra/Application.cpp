#include "Application.h"

#include "Mantra/Renderer/Renderer.h"

namespace Mantra {
Application* Application::sInstance = nullptr;

Application::Application() {
    if (sInstance) {
        ME_CORE_ERROR("Trying to create another Application instance.");
        assert(true);
        return;
    }

    sInstance = this;

    mWindow = std::unique_ptr<Window>(Window::Create());
    mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    mImGuiLayer = new ImGuiLayer();
    PushOverlay(mImGuiLayer);

    mTriangleVA.reset(VertexArray::Create());

    float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                             0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                             0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    std::shared_ptr<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    BufferLayout layout = {{ShaderDataType::Float3, "a_Position"}, {ShaderDataType::Float4, "a_Color"}};
    vertexBuffer->SetLayout(layout);
    mTriangleVA->AddVertexBuffer(vertexBuffer);

    uint32_t indices[3] = {0, 1, 2};
    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    mTriangleVA->SetIndexBuffer(indexBuffer);

    mSquareVA.reset(VertexArray::Create());

    float squareVertices[3 * 4] = {-0.75f, -0.75f, 0.0f, 0.75f, -0.75f, 0.0f, 0.75f, 0.75f, 0.0f, -0.75f, 0.75f, 0.0f};

    std::shared_ptr<VertexBuffer> squareVB;
    squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetLayout({{ShaderDataType::Float3, "a_Position"}});
    mSquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<IndexBuffer> squareIB;
    squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    mSquareVA->SetIndexBuffer(squareIB);

    std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

    std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

    mTriangleShader.reset(new Shader(vertexSrc, fragmentSrc));

    std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

    std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.4, 1.0);
			}
		)";

    mSquareShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
}

Application::~Application() {
    // glfwTerminate();
}

void Application::Run() {
    while (mRunning) {

        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        Renderer::BeginScene();

        mSquareShader->Bind();
        Renderer::Submit(mSquareVA);

        mTriangleShader->Bind();
        Renderer::Submit(mTriangleVA);

        for (Layer* layer : mLayerstack) {
            layer->OnUpdate();
        }

        mImGuiLayer->Begin();
        for (Layer* layer : mLayerstack) {
            layer->OnImGuiRender();
        }
        mImGuiLayer->End();

        mWindow->OnUpdate();
    }
}
void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

    for (auto it = mLayerstack.end(); it != mLayerstack.begin();) {
        (*--it)->OnEvent(e);
        if (e.handled) {
            break;
        }
    }
}

void Application::PushLayer(Layer* layer) {
    mLayerstack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
    mLayerstack.PushOverlay(layer);
    layer->OnAttach();
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    mRunning = false;
    //TODO: cleanup glfw context
    return true;
}
}  // namespace Mantra