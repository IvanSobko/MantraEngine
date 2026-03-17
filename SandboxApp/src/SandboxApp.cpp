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
    ExampleLayer() : Layer("Example"), mCameraController(1280.0f / 720.0f) {

        mCameraController.GetCamera().SetPosition({0.0f, 1.0f, 5.0f});
        mCameraController.GetCamera().SetRotation({-20.0f, 0.0f, 0.0f});
        mShaderLibrary = std::make_unique<Mantra::ShaderLibrary>();

        mSquareVA = Mantra::VertexArray::Create();

        // Vertex format: position (3 floats), tex coords (2 floats)
        float squareVertices[5 * 4] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

                                       0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,

                                       0.5f,  0.5f,  0.0f, 1.0f, 1.0f,

                                       -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

        std::shared_ptr<Mantra::VertexBuffer> squareVB =
            Mantra::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
        squareVB->SetLayout({
            {Mantra::ShaderDataType::Float3, "a_Position"},
            {Mantra::ShaderDataType::Float2, "a_TexCoord"},
        });
        mSquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Mantra::IndexBuffer> squareIB =
            Mantra::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        mSquareVA->SetIndexBuffer(squareIB);

        mCubeVA = Mantra::VertexArray::Create();

        // Cube vertices: position (3 floats) for all 6 faces
        float cubeVertices[8 * 3] = {// Back face
                                     -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
                                     // Front face
                                     -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

        std::shared_ptr<Mantra::VertexBuffer> cubeVB = Mantra::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
        cubeVB->SetLayout({{Mantra::ShaderDataType::Float3, "a_Position"}});
        mCubeVA->AddVertexBuffer(cubeVB);

        // Cube indices for all 6 faces (12 triangles)
        uint32_t cubeIndices[36] = {// Back face
                                    0, 1, 2, 2, 3, 0,
                                    // Front face
                                    4, 5, 6, 6, 7, 4,
                                    // Left face
                                    7, 3, 0, 0, 4, 7,
                                    // Right face
                                    1, 5, 6, 6, 2, 1,
                                    // Bottom face
                                    0, 1, 5, 5, 4, 0,
                                    // Top face
                                    3, 2, 6, 6, 7, 3};

        std::shared_ptr<Mantra::IndexBuffer> cubeIB =
            Mantra::IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
        mCubeVA->SetIndexBuffer(cubeIB);

        std::string cubeVertexSrc = R"(
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

        std::string cubeFragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
            uniform vec4 u_Color;
			in vec3 v_Position;
			void main()
			{
				color = u_Color;
			}
		)";

        mShaderLibrary->Add(std::make_shared<Mantra::OpenGLShader>("cube", cubeVertexSrc, cubeFragmentSrc));
        mShaderLibrary->Load("../assets/shaders/texture.glsl");

        mShaderLibrary->Get("texture")->Bind();
        mShaderLibrary->Get("texture")->SetUniformInt("u_Texture", 0);

        mRGBTexture = Mantra::Texture2D::Create("../assets/checkerboard.png");
        // mRGBATexture = Mantra::Texture2D::Create("../assets/logo.png");
    }

    void OnUpdate(Mantra::Timestep ts) override {
        mShaderLibrary->Get("cube")->Bind();
        mShaderLibrary->Get("cube")->SetUniformFloat4("u_Color", mSquareColor);

        Mantra::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mantra::RenderCommand::Clear();

        mCameraController.OnUpdate(ts);

        // Replace the grid rendering loop with a single large cube
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));  // Make it full size instead of 0.1f

        Mantra::Renderer::BeginScene(mCameraController.GetCamera());

        // Render one big cube instead of the grid
        glm::vec3 pos(0.0f, 0.0f, 0.0f);  // Center it at origin
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
        Mantra::Renderer::Submit(mShaderLibrary->Get("cube"), mCubeVA, transform);

        glm::mat4 texTransform = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));

        mRGBTexture->Bind();
        Mantra::Renderer::Submit(mShaderLibrary->Get("texture"), mSquareVA, texTransform);

        Mantra::Renderer::EndScene();
    }

    void OnImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));
        ImGui::End();
    }

    void OnEvent(Mantra::Event& event) override { mCameraController.OnEvent(event); }

private:
    Mantra::PerspectiveCameraController mCameraController;

    glm::vec4 mSquareColor = {0.2f, 0.3f, 0.4f, 1.0f};

    std::unique_ptr<Mantra::ShaderLibrary> mShaderLibrary;

    std::shared_ptr<Mantra::VertexArray> mSquareVA;
    std::shared_ptr<Mantra::VertexArray> mCubeVA;
    std::shared_ptr<Mantra::Texture2D> mRGBTexture;  //, mRGBATexture;
};

class Sandbox : public Mantra::Application
{
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
        SetVSync(true);
    }

    ~Sandbox() {}

private:
};

Mantra::Application* Mantra::CreateApplication() {
    return new Sandbox();
}
