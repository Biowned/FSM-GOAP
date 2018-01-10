#include "..\src\Graph.h"





Edge::Edge(Vector2D _first, Vector2D _second, float _cost) {
	cost = _cost;
	first = _first;
	second = _second;
}
//Edge::Edge() {
//
//}
Edge::~Edge() {

}
