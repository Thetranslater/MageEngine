#pragma once

namespace Mage {

	class MageEngine {
	public:
		MageEngine() = default;

		void startEngine();
		void shutdownEngine();

		void run();
	private:
		void logicalTick(float delta_time);
		void renderTick(float delta_time);
	};
}