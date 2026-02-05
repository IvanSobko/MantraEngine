#include <MantraEngine.h>

#include <iostream>
#include "imgui.h"

class ExampleLayer : public Mantra::Layer
{
public:
    ExampleLayer() : Layer("Example") {
        mTriangleVA.reset(Mantra::VertexArray::Create());

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

        float squareVertices[3 * 4] = {-0.75f, -0.75f, 0.0f, 0.75f,  -0.75f, 0.0f,
                                       0.75f,  0.75f,  0.0f, -0.75f, 0.75f,  0.0f};

        std::shared_ptr<Mantra::VertexBuffer> squareVB;
        squareVB.reset(Mantra::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({{Mantra::ShaderDataType::Float3, "a_Position"}});
        mSquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Mantra::IndexBuffer> squareIB;
        squareIB.reset(Mantra::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

        mTriangleShader.reset(new Mantra::Shader(vertexSrc, fragmentSrc));

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

        mSquareShader.reset(new Mantra::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    void OnUpdate() override {
        Mantra::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mantra::RenderCommand::Clear();

        Mantra::Renderer::BeginScene();

        mSquareShader->Bind();
        Mantra::Renderer::Submit(mSquareVA);

        mTriangleShader->Bind();
        Mantra::Renderer::Submit(mTriangleVA);
    }

    void OnEvent(Mantra::Event& event) override {}

private:
    std::shared_ptr<Mantra::Shader> mTriangleShader;
    std::shared_ptr<Mantra::VertexArray> mTriangleVA;

    std::shared_ptr<Mantra::Shader> mSquareShader;
    std::shared_ptr<Mantra::VertexArray> mSquareVA;
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
