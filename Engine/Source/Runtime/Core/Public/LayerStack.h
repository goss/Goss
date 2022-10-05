#pragma once

#include "Layer.h"

#include <vector>

namespace Goss {

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator Begin() { return layers.begin(); }
		std::vector<Layer*>::iterator End() { return layers.end(); }
		std::vector<Layer*>::reverse_iterator ReverseBegin() { return layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator ReverseEnd() { return layers.rend(); }

		std::vector<Layer*>::const_iterator Begin() const { return layers.begin(); }
		std::vector<Layer*>::const_iterator End()	const { return layers.end(); }
		std::vector<Layer*>::const_reverse_iterator ReverseBegin() const { return layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator ReverseEnd() const { return layers.rend(); }
	private:
		std::vector<Layer*> layers;
		unsigned int layerInsertIndex = 0;
	};

}