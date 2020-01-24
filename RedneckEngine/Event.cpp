#include "Event.h"


std::unordered_map<std::type_index, std::vector<EventDispatcher::EventListener>> EventDispatcher::m_GlobalCallbacks;