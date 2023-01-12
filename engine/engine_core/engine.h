#pragma once

namespace Mage {

	class MageEngine {
	public:
		MageEngine() = default;

		void startEngine();
		void shutdownEngine();

		bool tick(float delta);
	private:
		void logicalTick(float delta);
		void renderTick(float delta);
	};
}