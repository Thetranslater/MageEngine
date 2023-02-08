#pragma once

namespace Mage {

	class MageEngine {
	public:
		MageEngine() = default;

		void startEngine();
		//post-initialization after finish initializing editor
		void postInitialize();
		void shutdownEngine();

		bool tick(float delta);
	private:
		void logicalTick(float delta);
		void renderTick(float delta);
	};
}