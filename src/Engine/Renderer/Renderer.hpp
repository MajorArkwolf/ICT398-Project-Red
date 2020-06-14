#pragma once
#include <utility>
#include <vector>

namespace View {
    class Renderer {
        /**
         * Initialises the renderer.
         */
        virtual void Init() = 0;
        /**
         * The default draw interface.
         */
        virtual void Draw() = 0;
        /**
         * The default de init interface.
         */
        virtual void DeInit() = 0;
        /**
         * The default toggle wire frame interface.
         */
        virtual void ToggleWireFrame() = 0;

        /**
         * Updates the view port
         * @param bl Bottom left
         * @param br Bottom right
         * @param tl Top Left
         * @param tr Top Right
         */
        virtual void UpdateViewPort(int bl, int br, int tl, int tr) = 0;
    };
}
