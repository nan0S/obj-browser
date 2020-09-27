# OBJ Browser
**OBJ file browser** with support of applying texture and noise to the object and inspecting it. It supports **OBJ** file type for objects, **DDS** and **BMP** file types for textures. This browser also allows to apply Perlin Noise either with or without texture.

#### Navigation
##### General
Use your mouse or arrows to rotate the object. Use the mouse scroll or the Z/X buttons to zoom in/out. Other buttons:
* T - turn on the texture mode (texture is applied to the object)
* G - turn on the Perlin Noise mode (Perlin Noise is applied to the object, texture is turned off)
* B - turn on the mixed mode (texture and Perlin Noise are applied simultaneously)
##### Lightning
* F - toggle lightning
* W - increase light power
* S - decrease light power
##### Perlin Noise
Perlin Noise is evaluated using the formula below:
$$
pnoise(t) = \sum_{i=0}^{octaves-1}(persistance^i * noise(t * scale * lacunarity^i))
$$
where $noise(x)$ is original value of the Perlin Noise.
* Y - increase scale
* H - decrease scale
* U - increase persistance
* J - decrease persistance
* I - increase lacunarity
* K - decrease lacunarity
* O - increase octaves
* L - decrease octaves
* A - toggle animation mode (if turned on, t increases with time, otherwise not)

Screenshots
---
### Suzanne with texture
![](https://i.imgur.com/bZDY0Ph.png)

### Suzanne with Perlin Noise
![](https://i.imgur.com/hP6oeeL.png)

### Tank
![](https://i.imgur.com/yUILAQu.png)

### Colorful car with Perlin Noise
![](https://i.imgur.com/ogk7cZW.png)