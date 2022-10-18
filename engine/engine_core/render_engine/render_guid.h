#pragma once

namespace Mage {
	using GUID32 = unsigned int;
	using GUID64 = unsigned long long;

	const GUID32 invalid_guid32{ 0xffffffffui32 };
	const GUID64 invalid_guid64{ 0xffffffffffffffffui64 };
}