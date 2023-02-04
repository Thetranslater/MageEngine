#pragma once

namespace Mage {
	class MageEngine;
	class MageEditor {
	public:
		MageEditor() = default;

		void initialize(MageEngine*);

		void run();

		void shutdown();

	private:
		void initializeUI();

		MageEngine* m_engine{ nullptr };
	};
}