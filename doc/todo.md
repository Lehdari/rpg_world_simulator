## Architecture

Changes that affect the usage interface from the logic implementation point of view

- Generic entity interface for `World`
  - Currently all entities are clumsily in their respective vectors
  - Lot of copy-paste required for creating / removing / updating different entities
  - Perhaps a `std::tuple` of entity vectors, similar to components in `ComponentPool` would work?

- Add global simulation parameters
  - Controllable via the GUI?

## Optimization

- Structure for efficient spatial queries
  - Currently performing entity location queries involves iterating over all entities
    - Example query: "Give me all entities within radius of `r` around point `p`"
  - Implement a grid-based cache that stores handles to `Orientation` components
    - Updated at the beginning of each cycle (linear operation so it shouldn't be too expensive)
    - Cell size larger than the largest `CollisionBody` so that the structure can be used for collision checking
- Refactor collision checking so it uses the optimization structure above