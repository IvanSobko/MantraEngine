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

        CreateGrid();
    }

    void OnUpdate(Mantra::Timestep ts) override {

        Mantra::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mantra::RenderCommand::Clear();

        mCameraController.OnUpdate(ts);

        // Replace the grid rendering loop with a single large cube
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));  // Make it full size instead of 0.1f

        Mantra::Renderer::BeginScene(mCameraController.GetCamera());

        auto gridShader = mShaderLibrary->Get("grid");
        gridShader->Bind();
        gridShader->SetUniformFloat("u_GridSize", mGridSize);
        gridShader->SetUniformFloat("u_LineWidth", mLineWidth);
        gridShader->SetUniformFloat4("u_GridColor", mGridColor);
        gridShader->SetUniformFloat4("u_AxisColor", mAxisColor);
        Mantra::Renderer::Submit(gridShader, mGridVA, glm::mat4(1.0f));

        // Render one big cube
        mShaderLibrary->Get("cube")->Bind();
        mShaderLibrary->Get("cube")->SetUniformFloat4("u_Color", mSquareColor);
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
        ImGui::Separator();
        ImGui::Text("Grid Settings");
        ImGui::SliderFloat("Grid Size", &mGridSize, 0.1f, 10.0f);
        ImGui::SliderFloat("Line Width", &mLineWidth, 0.01f, 0.5f);
        ImGui::ColorEdit4("Grid Color", glm::value_ptr(mGridColor));
        ImGui::ColorEdit4("Axis Color", glm::value_ptr(mAxisColor));
        ImGui::End();
    }

    void OnEvent(Mantra::Event& event) override { mCameraController.OnEvent(event); }

    void CreateGrid() {
        mGridVA = Mantra::VertexArray::Create();
        // Large quad for the infinite grid (positioned at y=0 for x-z plane)
        float gridVertices[5 * 4] = {
            -1000.0f, 0.0f, -1000.0f, -1000.0f, -1000.0f,  // Bottom-left
            1000.0f,  0.0f, -1000.0f, 1000.0f,  -1000.0f,  // Bottom-right
            1000.0f,  0.0f, 1000.0f,  1000.0f,  1000.0f,   // Top-right
            -1000.0f, 0.0f, 1000.0f,  -1000.0f, 1000.0f    // Top-left
        };

        std::shared_ptr<Mantra::VertexBuffer> gridVB = Mantra::VertexBuffer::Create(gridVertices, sizeof(gridVertices));
        gridVB->SetLayout({
            {Mantra::ShaderDataType::Float3, "a_Position"},
            {Mantra::ShaderDataType::Float2, "a_WorldPos"},  // World position for grid calculation
        });
        mGridVA->AddVertexBuffer(gridVB);

        uint32_t gridIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Mantra::IndexBuffer> gridIB =
            Mantra::IndexBuffer::Create(gridIndices, sizeof(gridIndices) / sizeof(uint32_t));
        mGridVA->SetIndexBuffer(gridIB);

        // Grid shader
        std::string gridVertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_WorldPos;
            
            uniform mat4 u_ViewProjection;
            
            out vec2 v_WorldPos;
            
            void main()
            {
                v_WorldPos = a_WorldPos;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string gridFragmentSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;
            
            in vec2 v_WorldPos;
            
            uniform float u_GridSize;
            uniform float u_LineWidth;
            uniform vec4 u_GridColor;
            uniform vec4 u_AxisColor;
            
            float grid(vec2 pos, float size) {
                vec2 coord = pos / size;
                vec2 grid_coord = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
                float line = min(grid_coord.x, grid_coord.y);
                return 1.0 - min(line, 1.0);
            }
            
            void main()
            {
                float gridValue = grid(v_WorldPos, u_GridSize);
                
                // Create thicker lines for main axes (x=0 and z=0)
                float axisX = smoothstep(0.0, u_LineWidth * 2.0, abs(v_WorldPos.x));
                float axisZ = smoothstep(0.0, u_LineWidth * 2.0, abs(v_WorldPos.y));
                
                vec4 finalColor = mix(u_AxisColor, u_GridColor, min(axisX, axisZ));
                
                color = vec4(finalColor.rgb, gridValue * finalColor.a);
                
                if (color.a < 0.01) discard;
            }
        )";

        mShaderLibrary->Add(std::make_shared<Mantra::OpenGLShader>("grid", gridVertexSrc, gridFragmentSrc));
    }

private:
    Mantra::PerspectiveCameraController mCameraController;

    glm::vec4 mSquareColor = {0.2f, 0.3f, 0.4f, 1.0f};

    std::unique_ptr<Mantra::ShaderLibrary> mShaderLibrary;

    std::shared_ptr<Mantra::VertexArray> mSquareVA;
    std::shared_ptr<Mantra::VertexArray> mCubeVA;
    std::shared_ptr<Mantra::Texture2D> mRGBTexture;  //, mRGBATexture;

    //Grid settings
    float mGridSize = 1.0f;
    float mLineWidth = 0.05f;
    glm::vec4 mGridColor = {0.5f, 0.5f, 0.5f, 0.5f};
    glm::vec4 mAxisColor = {1.0f, 1.0f, 1.0f, 0.8f};
    std::shared_ptr<Mantra::VertexArray> mGridVA;
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
