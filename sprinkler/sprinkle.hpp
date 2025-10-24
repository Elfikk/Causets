#pragma once

#include "graphs/directed_graph.hpp"

#include <algorithm>
#include <functional>
#include <optional>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Sprinkle
{
public:
    Sprinkle(
        std::function<CausalRelation(const Event<d> &, const Event<d> &)> func,
        std::vector<Event<d>> specialPoints = {}
    ) : causalRelationFunc(func), specialPathPoints(specialPoints) {}
    void addEvent(Event<d> & event) { events.emplace_back(event); }

    DirectedGraph generateCausalSet();
    DirectedGraph generateSpecialPointCausalSet();

    std::vector<Event<d>> getEvents() { return events; }
    std::vector<std::size_t> getSpecialPointIds() { return specialPointIds; }

private:
    std::vector<Event<d>> events;
    std::function<CausalRelation(const Event<d> &, const Event<d> &)> causalRelationFunc;
    // At low N, we can introduce special endpoints to the sprinkle to get expected path lengths.
    std::vector<Event<d>> specialPathPoints;
    std::vector<std::size_t> specialPointIds = {};
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
DirectedGraph Sprinkle<d>::generateCausalSet()
{
    // Sort the events by time within the vector
    std::sort(
        events.begin(),
        events.end(),
        [](Event<d> a, Event<d> b) { return a.time() < b.time(); }
    );

    DirectedGraph graph = DirectedGraph(events.size());

    for (std::size_t i = 0; i < events.size() - 1; i++)
    {
        for (std::size_t j = i + 1; j < events.size(); j++)
        {
            auto relation = causalRelationFunc(events[i], events[j]);
            if (relation == CausalRelation::CausalFuture)
            {
                graph.addEdge(i, j);
            }
        }
    }
    return graph;
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
DirectedGraph Sprinkle<d>::generateSpecialPointCausalSet()
{
    auto eventsCopy = events;
    for (auto event : specialPathPoints)
    {
        eventsCopy.emplace_back(event);
    }

    // Sort the events by time within the vector
    std::sort(
        eventsCopy.begin(),
        eventsCopy.end(),
        [](Event<d> a, Event<d> b) { return a.time() < b.time(); }
    );

    DirectedGraph graph = DirectedGraph(eventsCopy.size());

    specialPointIds = {};
    for (std::size_t i = 0; i < eventsCopy.size(); i++)
    {
        if (std::find(specialPathPoints.begin(), specialPathPoints.end(), eventsCopy[i]) != specialPathPoints.end())
        {
            specialPointIds.emplace_back(i);
        }
    }

    for (std::size_t i = 0; i < eventsCopy.size() - 1; i++)
    {
        for (std::size_t j = i + 1; j < eventsCopy.size(); j++)
        {
            auto relation = causalRelationFunc(eventsCopy[i], eventsCopy[j]);
            if (relation == CausalRelation::CausalFuture)
            {
                graph.addEdge(i, j);
            }
        }
    }
    return graph;
}
