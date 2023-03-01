# MaterialBoard

This *MaterialBoard* library provides the necessary firmware to transform your supported Arduino microcontroller board into a *MaterialBoard*. 

In combination with the [VirtualBoard](https://github.com/virtual-maker/VirtualBoard) project, this makes it possible to run Arduino sketches "in vito" directly in the Visual Studio 2022 debugger on a Windows PC and interact at runtime with your real hardware circuit (via digital and analog pins, SPI and I2C). If the logic of the sketch code "in vito" works as expected, the sketch then can be loaded directly into the microcontroller board with the connected hardware circuit for the final tests.

The *MaterialBoard* library is a fork of the [ConfigurableFirmata](https://github.com/firmata/ConfigurableFirmata) project and uses the [Firmata protocol](https://github.com/firmata/protocol) internally for serial communication between the PC and the *MaterialBoard*. The reason for this fork is to make the installation of the required software on your microcontroller board as easy and hassle free as possible.

## Installation

- **If you are using Arduino IDE version 1.6.4 or higher** go to `Sketch > Include Library > Manage Libraries` and then search for "MaterialBoard" and click on `Install` after tapping on the MaterialBoard item in the filtered results. You can also use this same method to update MaterialBoard in the future.
- **If you are using an older version of the Arduino IDE**, download or clone MaterialBoard to your Arduino sketchbook library folder. This is typically `/Documents/Arduino/libraries/` on Mac or Linux or `\Documents\Arduino\libraries\` on Windows.
- **If you want to edit things yourself or help in development**, clone this repo to `\Documents\Arduino\libraries\MaterialBoard` and start hacking. Just delete the folder if it exists already.

## Usage

Open und upload the included [MaterialBoard.ino](https://github.com/virtual-maker/MaterialBoard/blob/main/examples/MaterialBoard/MaterialBoard.ino) example file to your Arduino compatible microcontroller board. Then you are ready to use the board with the VirtualBoard extension for Visual Studio 2022.

## Supported boards

MaterialBoard is supported on a large variety of boards, both on 8-Bit microcontrollers such as the AVR-Based Arduinos as well as on 32-Bit microcontrollers such as the ESP32, Arduino Due or Raspberry Pi Pico.

For details on particular boards see [this page](BoardSupport.md).

## Contributing

If you discover a bug or would like to propose a new feature, please open a new [issue](https://github.com/virtual-maker/MaterialBoard/issues?sort=created&state=open).

To contribute, fork this repository and create a new topic branch for the bug, feature or other existing issue you are addressing. Submit the pull request against the *main* branch.

You must thoroughly test your contributed code. In your pull request, describe tests performed to ensure that no existing code is broken and that any changes maintain backwards compatibility with the existing API. Test on multiple Arduino board variants if possible. We hope to enable some form of automated (or at least semi-automated) testing in the future, but for now any tests will need to be executed manually by the contributor and reviewers.

Use [Artistic Style](http://astyle.sourceforge.net/) (astyle) to format your code. Set the following rules for the astyle formatter:

```
style = ""
indent = "spaces"
indent-spaces = 2
indent-classes = true
indent-switches = true
indent-cases = true
indent-col1-comments = true
pad-oper = true
pad-header = true
keep-one-line-statements = true
```

If you happen to use Sublime Text, [this astyle plugin](https://github.com/timonwong/SublimeAStyleFormatter) is helpful. Set the above rules in the user settings file.
