#pragma once

#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/Controllers.h"
#include "Symbols.h"

namespace Pinetime {
    namespace Applications {
        namespace Screens {

            class InfiniSleep : public Screen {
            public:
                InfiniSleep();
                ~InfiniSleep();
            };
        }

    template <>
    struct AppTraits<Apps::InfiniSleep> {
      static constexpr Apps app = Apps::InfiniSleep;
      static constexpr const char* icon = Screens::Symbols::sleep;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::InfiniSleep();
      };
    };
    }
}