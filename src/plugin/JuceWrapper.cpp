//==============================================================================
/*
  This is the entry point for JUCE standalone apps.
  It creates the necessary factory function for the plugin.
*/

#include "DistortionPro.h"

// Create the plugin instance - required by JUCE standalone wrapper
juce::AudioProcessor* createPluginFilter() {
    return new DistortionPro::DistortionPro();
}
