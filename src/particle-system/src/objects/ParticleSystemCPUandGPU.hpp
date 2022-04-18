#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "../renderer/VertexBuffer.hpp"
#include "../renderer/VertexArray.hpp"
#include "../renderer/Shader.hpp"
#include "../renderer/DrawData.hpp"
#include "ParticleSystemCPU.hpp"

class ParticleSystemCPUandGPU : public ParticleSystemCPU
{
private:
	class Data : public DrawData
	{
	private:
		float* vertices;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		VertexArray vertexArray;

		size_t maxVertexCount;
		size_t stride;

		size_t vertexIndex = 0;
		size_t offset = 0;
	public:
		Data(size_t quadCount);
		~Data();

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetColor(const glm::vec4& color);
		void SetSize(float size);
		void NextVertex();

		bool CanDraw();
		bool IsFull();

		void Draw();
		void Reset();
	};

	Data* data;
	Shader shader;

	void Draw();
public:
	ParticleSystemCPUandGPU(
		const std::string& name,
		unsigned int poolSize,
		unsigned int particlesPerFrame,
		ParticleProperties particleProperties,
		unsigned int maxQuadInBatch = 25000
	);
	~ParticleSystemCPUandGPU();

	void RenderParticle(const ParticleSystemCPU::Particle *particle, float k) override;
	void OnRenderEnd() override;
};

