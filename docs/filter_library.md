# Filter Library

## Color Filters

Color filters manipulate each pixel in isolation.

### Clear

Clears one or more channels of an existing image with a particular color.

| Argument | Description|
|-|-|
|`color`|The color to use to clear the image.|
|`clear_red`| If the red channel of the image should be cleared.|
|`clear_green`| If the green channel of the image should be cleared.|
|`clear_blue`| If the blue channel of the image should be cleared.|
|`clear_alpha`| If the alpha channel of the image should be cleared.|

![Clear Filter](assets/clear.png)
### Grayscale

Converts the RGB channels of an image to grayscale. This filter takes no arguments.
![Grayscale Filter](assets/grayscale.png)
### Invert

Inverts the RGB channels of an image. This filter takes no arguments.
![Invert Filter](assets/invert.png)

