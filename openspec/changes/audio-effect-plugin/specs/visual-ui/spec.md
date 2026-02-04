## ADDED Requirements

### Requirement: Waveform visualization
The UI SHALL display real-time input and output waveforms.

#### Scenario: Waveform shows audio activity
- **WHEN** audio is processing
- **THEN** waveform display updates at least 30 times per second
- **AND** input waveform shows signal before distortion
- **AND** output waveform shows signal after distortion
- **AND** both waveforms are synchronized

#### Scenario: Waveform responds to signal changes
- **WHEN** input signal level changes from -20 dB to -6 dB
- **THEN** waveform display reflects increased amplitude
- **AND** no visual artifacts or tearing occur

### Requirement: Gain reduction meter
The UI SHALL display gain reduction during compression/limiting stages.

#### Scenario: Gain meter shows reduction amount
- **WHEN** distortion is applied
- **THEN** gain reduction meter shows current reduction in dB
- **AND** meter updates in real-time with minimal lag

### Requirement: Parameter controls
The UI SHALL provide controls for all distortion parameters.

#### Scenario: User adjusts drive knob
- **WHEN** user clicks and drags drive knob
- **THEN** parameter value changes smoothly
- **AND** value is displayed numerically
- **AND** knob visual feedback matches value

#### Scenario: User selects distortion type
- **WHEN** user opens type dropdown
- **THEN** available types are listed
- **AND** selection updates distortion algorithm

### Requirement: Responsive layout
The UI SHALL maintain proper layout at various window sizes.

#### Scenario: UI scales for different window sizes
- **WHEN** user resizes plugin window
- **THEN** all elements scale proportionally
- **AND** minimum width of 400 pixels is respected
- **AND** controls remain usable at any size

### Requirement: Dark theme
The UI SHALL use a professional dark theme.

#### Scenario: Default appearance is dark
- **WHEN** plugin is first loaded
- **THEN** UI uses dark background colors
- **AND** controls have high contrast for visibility
- **AND** meters display clearly against dark background
