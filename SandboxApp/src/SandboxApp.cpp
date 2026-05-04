#include <Mantra/EntryPoint.h>
#include <MantraEngine.h>

#include <glad/glad.h>
#include <deque>
#include <iostream>
#include "imgui.h"

#include "Mantra/Renderer/OpenGL/OpenGLShader.h"
#include "Mantra/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Mantra::Layer
{
public:
    ExampleLayer() : Layer("Example") {
        float aspectRatio = 1280.0f / 720.0f;
        mCamera = std::make_unique<Mantra::EditorCamera>(85.0f, aspectRatio, 0.01f, 1000.0f);
        // mCamera = std::make_unique<Mantra::OrthoCamera>(-aspectRatio, aspectRatio, -1.0f, 1.0f, -100.0f, 100.0f);

        mGPUResources = std::make_unique<Mantra::GPUResourceManager>();

        Mantra::FramebufferSpecification spec;
        spec.height = 720;
        spec.width = 1280;
        spec.attachments = Mantra::FramebufferAttachment(
            {Mantra::FramebufferFormat::RGBA8, Mantra::FramebufferFormat::DEPTH24STENCIL8});
        mFramebuffer = Mantra::Framebuffer::Create(spec);

        CreateCube();

        int meshID = mScene.LoadAndAddMesh("../assets/models/monkey.obj");
        ME_INFO("Loaded mesh with ID {0}", meshID);
        Mantra::Material material;
        material.name = "MonkeyMaterial";
        material.shaderName = "wireframe";
        material.baseColor = {1.0f, 1.0f, 1.0f, 1.0f};
        mScene.meshes[meshID].materialID = mScene.AddMaterial(material);

        Mantra::MeshInstance meshInstance;
        meshInstance.meshID = meshID;
        meshInstance.transform.translation = {0.0f, 0.0f, 4.0f};
        meshInstance.transform.rotation = {0.0f, 0.0f, 0.0f};
        meshInstance.transform.scale = {1.0f, 1.0f, 1.0f};
        mScene.AddInstance(meshInstance);
        mGPUResources->CreateVertexArrayFromMesh(meshID, mScene.meshes[meshID]);

        mGPUResources->LoadShader("texture", "../assets/shaders/texture.glsl");
        mGPUResources->LoadShader("normal", "../assets/shaders/normal.glsl");
        mGPUResources->LoadShader("wireframe", "../assets/shaders/wireframe.glsl");

        CreateGrid();
    }

    void OnUpdate(Mantra::Timestep ts) override {
        mFramebuffer->Bind();
        Mantra::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mantra::RenderCommand::Clear();
        mCamera->OnUpdate(ts);

        Mantra::Renderer::BeginScene(*mCamera);

        Mantra::Renderer::SetFrameTime(ts);

        auto gridShader = mGPUResources->GetShader("grid");
        gridShader->Bind();
        gridShader->SetUniformFloat("u_GridSize", mGridSize);
        gridShader->SetUniformFloat("u_LineWidth", mLineWidth);
        gridShader->SetUniformFloat4("u_GridColor", mGridColor);
        gridShader->SetUniformFloat4("u_AxisColor", mAxisColor);
        Mantra::Renderer::Submit(gridShader, mGridVA, glm::mat4(1.0f));

        // Render scene instances
        for (const auto& instance : mScene.instances) {
            const auto& mesh = mScene.meshes[instance.meshID];
            const auto& material = mScene.materials[mesh.materialID];

            auto vertexArray = mGPUResources->GetVertexArray(instance.meshID);
            auto shader = mGPUResources->GetShader(material.shaderName);

            if (!vertexArray || !shader)
                continue;

            // texture shader requires a texture; bind it
            if (material.shaderName == "texture") {
                auto texture = mGPUResources->GetTexture(material.baseColorTextureID);
                if (!texture)
                    continue;  // Skip if no texture
                texture->Bind();
            }

            glm::mat4 transform = BuildTransform(instance.transform);
            Mantra::Renderer::Submit(shader, vertexArray, transform);
        }

        Mantra::Renderer::EndScene();
        mFramebuffer->Unbind();
    }

    void OnImGuiRender() override {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::Begin("Viewport");

        ImVec2 avail = ImGui::GetContentRegionAvail();
        if (avail.x > 0 && avail.y > 0) {
            // Resize framebuffer when ImGui viewport size changes
            uint32_t newW = (uint32_t)avail.x;
            uint32_t newH = (uint32_t)avail.y;
            if (newW != (uint32_t)mViewportSize.x || newH != (uint32_t)mViewportSize.y) {
                // CreateFramebuffer(newW, newH);
                mFramebuffer->Resize(newW, newH);
                mViewportSize = {newW, newH};
                mCamera->SetViewportSize(newW, newH);
            }

            // ImGui expects a void* texture id for OpenGL textures:
            ImGui::Image((void*)(intptr_t)mFramebuffer->GetColorAttachmentRendererID(), avail, ImVec2(0, 1),
                         ImVec2(1, 0));
        }

        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::Text("Grid Settings");
        ImGui::SliderFloat("Grid Size", &mGridSize, 0.1f, 10.0f);
        ImGui::SliderFloat("Line Width", &mLineWidth, 0.01f, 0.5f);
        ImGui::ColorEdit4("Grid Color", glm::value_ptr(mGridColor));
        ImGui::ColorEdit4("Axis Color", glm::value_ptr(mAxisColor));

        ImGui::Separator();

        glm::vec3 camPos = mCamera->GetPosition();
        glm::vec3 camRot = mCamera->GetRotation();
        camRot = glm::degrees(camRot);  // Convert to degrees for display

        if (ImGui::InputFloat3("Camera Position (X, Y, Z):", glm::value_ptr(camPos))) {
            mCamera->SetPosition(camPos);
        }
        if (ImGui::InputFloat3("Camera Rotation (Euler angles):", glm::value_ptr(camRot))) {
            mCamera->SetRotation(glm::radians(camRot));
        }

        if (ImGui::Button("Reset Camera")) {
            mCamera->ResetView();
        }

        ImGui::End();

        ImGui::Begin("Statistics");
        auto frameStats = Mantra::Renderer::GetFrameStats();
        mFpsHistory.push_back(frameStats.fps);
        if (mFpsHistory.size() > 120) {
            mFpsHistory.pop_front();
        }

        ImGui::Text("Frame Stats:");
        ImGui::Text("  FPS: %.1f", frameStats.fps);
        ImGui::Text("  Draw Calls: %u", frameStats.drawCalls);
        ImGui::Text("  Vertices: %u", frameStats.verticesRendered);
        ImGui::Text("  Triangles: %u", frameStats.trianglesRendered);
        ImGui::Separator();

        // FPS Chart
        if (mFpsHistory.size() > 0) {
            std::vector<float> fpsValues(mFpsHistory.begin(), mFpsHistory.end());
            ImGui::PlotLines("FPS", fpsValues.data(), (int)fpsValues.size(), 0, nullptr, 0.0f, 200.0f, ImVec2(0, 80));
        }

        ImGui::Separator();
        ImGui::Text("Resource Counts:");
        ImGui::Text("  Meshes: %u", mGPUResources->GetMeshCount());
        ImGui::Text("  Textures: %u", mGPUResources->GetTextureCount());
        ImGui::Text("  Shaders: %u", mGPUResources->GetShaderCount());
        ImGui::End();

        ImGui::End();
    }

    void OnEvent(Mantra::Event& event) override { mCamera->OnEvent(event); }

    void CreateCube() {
        Mantra::Mesh cubeMesh;
        cubeMesh.name = "Cube";

        cubeMesh.vertices = {
            // Front (+Z)
            {{-0.5f, -0.5f, 0.5f}, {}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.5f}, {}, {0.0f, 1.0f}},

            // Back (-Z)
            {{0.5f, -0.5f, -0.5f}, {}, {0.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {}, {1.0f, 0.0f}},
            {{-0.5f, 0.5f, -0.5f}, {}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f}, {}, {0.0f, 1.0f}},

            // Left (-X)
            {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.5f}, {}, {1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f}, {}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {}, {0.0f, 1.0f}},

            // Right (+X)
            {{0.5f, -0.5f, 0.5f}, {}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f}, {}, {0.0f, 1.0f}},

            // Top (+Y)
            {{-0.5f, 0.5f, 0.5f}, {}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {}, {0.0f, 1.0f}},

            // Bottom (-Y)
            {{-0.5f, -0.5f, -0.5f}, {}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {}, {1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {}, {1.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.5f}, {}, {0.0f, 1.0f}},
        };

        cubeMesh.indices = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                            12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

        Mantra::SceneID cubeMeshID = mScene.AddMesh(cubeMesh);

        // Load a texture from disk
        Mantra::SceneID textureID =
            mScene.LoadAndAddTexture("../assets/checkerboard.png", Mantra::TextureSemantic::BaseColor, true);

        Mantra::Material cubeMaterial;
        cubeMaterial.name = "CubeMaterial";
        cubeMaterial.shaderName = "texture";
        cubeMaterial.baseColor = {1.0f, 1.0f, 1.0f, 1.0f};
        cubeMaterial.baseColorTextureID = textureID;

        mScene.meshes[cubeMeshID].materialID = mScene.AddMaterial(cubeMaterial);

        Mantra::MeshInstance cubeInstance;
        cubeInstance.meshID = cubeMeshID;
        cubeInstance.transform.translation = {0.0f, 0.0f, 0.0f};
        cubeInstance.transform.rotation = {0.0f, 0.0f, 0.0f};
        cubeInstance.transform.scale = {1.0f, 1.0f, 1.0f};
        mScene.AddInstance(cubeInstance);

        cubeInstance.transform.translation = {2.0f, 0.0f, 0.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.translation = {-2.0f, 0.0f, 0.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.translation = {0.0f, 2.0f, 0.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.translation = {0.0f, -2.0f, 0.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.translation = {0.0f, 0.0f, 2.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.translation = {0.0f, 0.0f, -2.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.rotation = {glm::radians(45.0f), glm::radians(45.0f), 0.0f};
        cubeInstance.transform.translation = {2.0f, 2.0f, 0.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.translation = {-2.0f, -2.0f, 0.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.translation = {2.0f, -2.0f, 0.0f};
        mScene.AddInstance(cubeInstance);
        cubeInstance.transform.translation = {-2.0f, 2.0f, 0.0f};
        mScene.AddInstance(cubeInstance);

        // Create GPU resources indexed by SceneID
        mGPUResources->CreateVertexArrayFromMesh(cubeMeshID, mScene.meshes[cubeMeshID]);
        if (textureID != Mantra::kInvalidSceneID) {
            mGPUResources->CreateTextureFromAsset(textureID, mScene.textures[textureID]);
        }
    }

    glm::mat4 BuildTransform(const Mantra::SceneTransform& transform) {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), transform.translation);
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.scale);
        return translation * rotationZ * rotationY * rotationX * scale;
    }

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

        mGPUResources->LoadShader("grid", "../assets/shaders/grid.glsl");
    }

private:
    std::unique_ptr<Mantra::Camera> mCamera;
    Mantra::Scene mScene;

    std::unique_ptr<Mantra::GPUResourceManager> mGPUResources;

    //Grid settings
    float mGridSize = 1.0f;
    float mLineWidth = 0.05f;
    glm::vec4 mGridColor = {0.5f, 0.5f, 0.5f, 0.5f};
    glm::vec4 mAxisColor = {1.0f, 1.0f, 1.0f, 0.8f};
    std::shared_ptr<Mantra::VertexArray> mGridVA;

    // Camera info
    glm::vec3 mCameraPosition = {0.0f, 0.0f, 5.0f};
    glm::vec3 mCameraRotation = {0.0f, 0.0f, 0.0f};

    // Framebuffer for the ImGui viewport
    std::shared_ptr<Mantra::Framebuffer> mFramebuffer;
    glm::vec2 mViewportSize = {1280.0f, 720.0f};

    // FPS history for chart
    std::deque<float> mFpsHistory;
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
