#pragma once

namespace Mage {

	class MageEngine {
	public:
		MageEngine() = default;

		void startEngine();
		void shutdownEngine();

		void tick(float delta_time);
	private:
	};
}