#include "LayerStack.h"
#include "Layer.h"

LayerStack::LayerStack()
{
	_layerInsert = _layers.begin();
}

LayerStack::~LayerStack()
{
	for (Layer* layer : _layers)
	{
		delete layer;
	}
}

void LayerStack::PushLayer(Layer * layer)
{
	_layerInsert = _layers.emplace(_layerInsert, layer);
}

void LayerStack::PushOverlay(Layer * overlay)
{
	_layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer * layer)
{
	auto itterator = std::find(_layers.begin(), _layers.end(), layer);
	if (itterator != _layers.end())
	{
		_layers.erase(itterator);
		_layerInsert--;
	}
}

void LayerStack::PopOverlay(Layer * overlay)
{
	auto itterator = std::find(_layers.begin(), _layers.end(), overlay);
	if (itterator != _layers.end())
	{
		_layers.erase(itterator);
	}
}
