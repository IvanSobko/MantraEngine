#include <Mantra/EntryPoint.h>
#include <MantraEngine.h>

#include <iostream>
#include "imgui.h"

#include "Mantra/Renderer/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Mantra::Layer
{
public:
    ExampleLayer() : Layer("Example"), mCameraController(1280.0f / 720.0f, true) {

        mShaderLibrary = std::make_unique<Mantra::ShaderLibrary>();

        mTriangleVA.reset(Mantra::VertexArray::Create());

        // Vertex format: position (3 floats), color (4 floats)
        float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

                                 0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                                 0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

        std::shared_ptr<Mantra::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Mantra::VertexBuffer::Create(vertices, sizeof(vertices)));
        Mantra::BufferLayout layout = {{Mantra::ShaderDataType::Float3, "a_Position"},
                                       {Mantra::ShaderDataType::Float4, "a_Color"}};
        vertexBuffer->SetLayout(layout);
        mTriangleVA->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Mantra::IndexBuffer> indexBuffer;
        indexBuffer.reset(Mantra::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        mTriangleVA->SetIndexBuffer(indexBuffer);

        mSquareVA.reset(Mantra::VertexArray::Create());

        // Vertex format: position (3 floats), tex coords (2 floats)
        float squareVertices[5 * 4] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

                                       0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,

                                       0.5f,  0.5f,  0.0f, 1.0f, 1.0f,

                                       -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

        std::shared_ptr<Mantra::VertexBuffer> squareVB;
        squareVB.reset(Mantra::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            {Mantra::ShaderDataType::Float3, "a_Position"},
            {Mantra::ShaderDataType::Float2, "a_TexCoord"},
        });
        mSquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Mantra::IndexBuffer> squareIB;
        squareIB.reset(Mantra::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        mSquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

        mShaderLibrary->Add(std::make_shared<Mantra::OpenGLShader>("triangle", vertexSrc, fragmentSrc));

        std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

        std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

            uniform vec3 u_Color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

        mShaderLibrary->Add(
            std::make_shared<Mantra::OpenGLShader>("blueBoxes", blueShaderVertexSrc, blueShaderFragmentSrc));
        mShaderLibrary->Load("assets/shaders/texture.glsl");

        std::dynamic_pointer_cast<Mantra::OpenGLShader>(mShaderLibrary->Get("texture"))->Bind();
        std::dynamic_pointer_cast<Mantra::OpenGLShader>(mShaderLibrary->Get("texture"))->SetUniformInt("u_Texture", 0);

        mRGBTexture = Mantra::Texture2D::Create("assets/checkerboard.png");
        mRGBATexture = Mantra::Texture2D::Create("assets/logo.png");
    }

    void OnUpdate(Mantra::Timestep ts) override {
        std::dynamic_pointer_cast<Mantra::OpenGLShader>(mShaderLibrary->Get("blueBoxes"))->Bind();
        std::dynamic_pointer_cast<Mantra::OpenGLShader>(mShaderLibrary->Get("blueBoxes"))
            ->SetUniformFloat3("u_Color", mSquareColor);

        Mantra::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mantra::RenderCommand::Clear();

        mCameraController.OnUpdate(ts);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        Mantra::Renderer::BeginScene(mCameraController.GetCamera());

        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Mantra::Renderer::Submit(mShaderLibrary->Get("blueBoxes"), mSquareVA, transform);
            }
        }

        // Mantra::Renderer::Submit(mTriangleShader, mTriangleVA);

        glm::mat4 texTransform = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));

        mRGBTexture->Bind();
        Mantra::Renderer::Submit(mShaderLibrary->Get("texture"), mSquareVA, texTransform);

        texTransform = glm::translate(texTransform, {0.0f, 0.0f, 0.1f});
        mRGBATexture->Bind();
        Mantra::Renderer::Submit(mShaderLibrary->Get("texture"), mSquareVA, texTransform);

        Mantra::Renderer::EndScene();
    }

    void OnImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(mSquareColor));
        ImGui::End();
    }

    void OnEvent(Mantra::Event& event) override { mCameraController.OnEvent(event); }

private:
    Mantra::OrthoCameraController mCameraController;

    glm::vec3 mSquareColor = {0.2f, 0.3f, 0.4f};

    std::unique_ptr<Mantra::ShaderLibrary> mShaderLibrary;

    std::shared_ptr<Mantra::VertexArray> mTriangleVA;
    std::shared_ptr<Mantra::VertexArray> mSquareVA;
    std::shared_ptr<Mantra::Texture2D> mRGBTexture, mRGBATexture;
};

class Sandbox : public Mantra::Application
{
public:
    Sandbox() { PushLayer(new ExampleLayer()); }

    ~Sandbox() {}

private:
};

Mantra::Application* Mantra::CreateApplication() {
    return new Sandbox();
}
