#include <vector>
#include <emscripten/bind.h>

struct Position {
    double lat;
    double lng;
};

std::vector<Position> positions;

void add_position(double lat, double lng) {
    positions.push_back({lat, lng});
}

const std::vector<Position>& get_positions() {
    return positions;
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("add_position", &add_position);
    emscripten::function("get_positions", &get_positions);
    emscripten::register_vector<Position>("VectorPosition");
    emscripten::value_object<Position>("Position")
        .field("lat", &Position::lat)
        .field("lng", &Position::lng);
}
