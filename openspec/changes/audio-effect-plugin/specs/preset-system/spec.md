## ADDED Requirements

### Requirement: Factory presets included
The plugin SHALL include factory-installed presets for immediate use.

#### Scenario: User loads factory preset
- **WHEN** user selects preset from factory category
- **THEN** all parameters set to preset values
- **AND** distortion algorithm matches preset design
- **AND** preset name is displayed

Factory presets SHALL include:
- **Vintage Overdrive**: Classic tube-style overdrive
- **British Crunch**: Rock-style moderate distortion
- **Hard Rock**: High-gain aggressive distortion
- **Fuzzy Math**: Experimental fuzz character
- **Clean Boost**: Subtle volume and harmonic enhancement
- **Studio Warmth**: Subtle tape-style saturation

### Requirement: User presets saveable
Users SHALL be able to save custom presets.

#### Scenario: User saves current settings as preset
- **WHEN** user clicks "Save Preset" and enters name
- **THEN** preset is stored in user preset folder
- **AND** preset appears in user category
- **AND** settings persist across plugin restarts

#### Scenario: User preset persists after restart
- **WHEN** DAW is restarted
- **THEN** previously saved user presets remain available
- **AND** user can load their saved presets

### Requirement: Preset import/export
Users SHALL be able to import and export presets.

#### Scenario: User exports preset to file
- **WHEN** user selects "Export Preset"
- **THEN** preset is saved to user-selected JSON file
- **AND** file contains all parameter values and preset name

#### Scenario: User imports preset from file
- **WHEN** user selects "Import Preset" and chooses file
- **THEN** preset is loaded and available in user category
- **AND** settings are applied correctly

### Requirement: Preset browsing
Users SHALL be able to browse presets by category.

#### Scenario: User filters by category
- **WHEN** user selects category filter
- **THEN** only presets in that category are displayed
- **AND** all presets available when no filter selected

### Requirement: Preset compare
Users SHALL be able to compare current settings against saved preset.

#### Scenario: User toggles A/B comparison
- **WHEN** user presses A/B switch
- **THEN** settings toggle between current and reference
- **AND** visual indicator shows which is active
