## ADDED Requirements

### Requirement: Distortion types supported
The distortion engine SHALL support the following distortion types:
- **Overdrive**: tanh-based soft clipping with smooth saturation and rich even harmonics
- **Distortion**: 5-stage hard clipping with aggressive character and odd harmonics
- **Fuzz**: Square wave synthesis with extreme gain and thick, fuzzy texture
- **Saturation**: Tape emulation with harmonic enhancement and subtle compression

#### Scenario: User selects overdrive type
- **WHEN** user selects "Overdrive" from distortion type dropdown
- **THEN** engine applies tanh-based soft clipping algorithm
- **AND** output maintains smooth harmonic content
- **AND** transition into clipping is gradual

#### Scenario: User selects distortion type
- **WHEN** user selects "Distortion" from distortion type dropdown
- **THEN** engine applies 5-stage hard clipping algorithm
- **AND** output exhibits aggressive, hard-edged character
- **AND** odd harmonics are emphasized

#### Scenario: User selects fuzz type
- **WHEN** user selects "Fuzz" from distortion type dropdown
- **THEN** engine applies square wave-based algorithm
- **AND** output exhibits characteristic thick, fuzzy texture
- **AND** extreme gain produces saturated harmonics

#### Scenario: User selects saturation type
- **WHEN** user selects "Saturation" from distortion type dropdown
- **THEN** engine applies tape emulation algorithm
- **AND** output exhibits warm, musical harmonics
- **AND** subtle compression smooths peaks

### Requirement: Core distortion parameters
The distortion engine SHALL provide the following adjustable parameters:
- **Drive**: Input gain level before clipping (0.0 to 1.0)
- **Tone**: High frequency dampening control (0.0 to 1.0)
- **Output**: Final output level (-inf to 0 dB)
- **Mix**: Dry/wet signal ratio (0.0 = full dry, 1.0 = full wet)
- **Depth**: Compression/softness depth (0.0 to 1.0)
- **Attack**: Response speed of distortion onset (0.0 to 1.0)

#### Scenario: Adjusting drive parameter
- **WHEN** user increases drive parameter from 0.3 to 0.7
- **THEN** input gain increases by 12 dB
- **AND** harmonic content increases proportionally

#### Scenario: Adjusting tone parameter
- **WHEN** user decreases tone parameter to 0.2
- **THEN** high frequencies are reduced by 80%
- **AND** output sounds warmer and less harsh

#### Scenario: Adjusting mix parameter
- **WHEN** user sets mix to 50%
- **THEN** output contains equal amounts of dry and wet signals
- **AND** blending maintains proper phase relationship

#### Scenario: Adjusting depth parameter
- **WHEN** user increases depth parameter
- **THEN** distortion exhibits softer knee characteristics
- **AND** transition into clipping becomes more gradual

#### Scenario: Adjusting attack parameter
- **WHEN** user decreases attack parameter
- **THEN** distortion onset becomes slower
- **AND** transient content passes before distortion engages

### Requirement: Low latency processing
The distortion engine SHALL process audio with maximum 5ms latency.

#### Scenario: Real-time processing within latency budget
- **WHEN** audio buffer size is 128 samples at 48kHz
- **THEN** total plugin latency SHALL NOT exceed 5ms
- **AND** processing completes before next buffer arrives

### Requirement: Oversampling support
The distortion engine SHALL optionally support 2x oversampling for reduced aliasing.

#### Scenario: Oversampling reduces aliasing artifacts
- **WHEN** oversampling is enabled
- **THEN** audio is upsampled to 2x sample rate
- **AND** distortion is applied at higher sample rate
- **AND** output is downsampled back to original rate
