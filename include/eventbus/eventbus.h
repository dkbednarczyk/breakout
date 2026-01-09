#pragma once

#include <functional>
#include <list>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <variant>

template <typename... TEvents>
class EventBus {
   public:
    using EventVariant = std::variant<TEvents...>;

   private:
    std::unordered_map<std::type_index,
                       std::list<std::function<void(const EventVariant&)>>>
        handlers;
    std::queue<EventVariant> eventQueue;

    template <typename T>
    void dispatchEvent(const T& event) {
        auto it = handlers.find(std::type_index(typeid(T)));

        if (it != handlers.end()) {
            const EventVariant eventVariant = event;
            for (auto& handler : it->second) {
                handler(eventVariant);
            }
        }
    }

   public:
    template <typename T>
    void Subscribe(std::function<void(const T&)> handler) {
        handlers[std::type_index(typeid(T))].push_back(
            [handler](const EventVariant& event) {
                handler(std::get<T>(event));
            });
    }

    template <typename T>
    void Publish(const T& event) {
        eventQueue.push(event);
    }

    template <typename T>
    void InstantPublish(const T& event) {
        dispatchEvent(event);
    }

    void ProcessEvents() {
        while (!eventQueue.empty()) {
            EventVariant event = std::move(eventQueue.front());
            eventQueue.pop();

            std::visit([this](const auto& e) { dispatchEvent(e); }, event);
        }
    }

    void Clear() {
        while (!eventQueue.empty()) {
            eventQueue.pop();
        }
    }
};
