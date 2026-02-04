#include "Application.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "ME_PCH.h"

namespace Mantra {
Application* Application::sInstance = nullptr;

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
        case Mantra::ShaderDataType::Float: return GL_FLOAT;
        case Mantra::ShaderDataType::Float2: return GL_FLOAT;
        case Mantra::ShaderDataType::Float3: return GL_FLOAT;
        case Mantra::ShaderDataType::Float4: return GL_FLOAT;
        case Mantra::ShaderDataType::Mat3: return GL_FLOAT;
        case Mantra::ShaderDataType::Mat4: return GL_FLOAT;
        case Mantra::ShaderDataType::Int: return GL_INT;
        case Mantra::ShaderDataType::Int2: return GL_INT;
        case Mantra::ShaderDataType::Int3: return GL_INT;
        case Mantra::ShaderDataType::Int4: return GL_INT;
        case Mantra::ShaderDataType::Bool: return GL_BOOL;
    }

    ME_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

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

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                             0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                             0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    mVertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

    {
        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };

        mVertexBuffer->SetLayout(layout);
    }

    uint32_t index = 0;
    const auto& layout = mVertexBuffer->GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
        index++;
    }

    uint32_t indices[3] = {0, 1, 2};
    mIndexBuffer.reset(IndexBuffer::Create(indices, 3));

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

    mShader.reset(new Shader(vertexSrc, fragmentSrc));
}

Application::~Application() {
    // glfwTerminate();
}

void Application::Run() {
    while (mRunning) {

        glClearColor(0.1f, 0.1f, 0.1f, 1);

        glClear(GL_COLOR_BUFFER_BIT);

        mShader->Bind();

        glBindVertexArray(mVertexArray);

        glDrawElements(GL_TRIANGLES, mIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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