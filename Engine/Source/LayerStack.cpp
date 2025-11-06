#include "BsPrecompileHeader.h"
#include "LayerStack.h"
#include "Layer.h"

LayerStack::LayerStack()
{
	
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
	_layers.emplace(_layers.begin() + _layerInsertIndex, layer);
	_layerInsertIndex++;
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
		_layerInsertIndex--;
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
