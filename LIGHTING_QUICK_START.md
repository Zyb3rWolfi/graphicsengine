# Quick Lighting Usage Guide

## Guide on the different light types

### In Application Constructor
```cpp
// Create a directional light (sun)
Light sun;
sun.LightType = LightType::DIRECTIONAL;
sun.direction = glm::normalize(glm::vec3(-1, -1, -1));
sun.ambient = glm::vec3(0.2f);
sun.diffuse = glm::vec3(0.8f);
sun.specular = glm::vec3(1.0f);

// Create a point light (torch)
Light torch;
torch.LightType = LightType::POINT;
torch.position = glm::vec3(5, 2, 0);
torch.ambient = glm::vec3(0.05f);
torch.diffuse = glm::vec3(1, 0.6f, 0.3f);  // Orange
torch.specular = glm::vec3(1, 0.8f, 0.5f);
torch.constant = 1.0f;
torch.linear = 0.09f;
torch.quadratic = 0.032f;

lights = { sun, torch };
```

### Result
The shader automatically dispatches to the correct lighting function based on `light.type`.

---

## Light Type Reference

```cpp
enum class LightType {
    DIRECTIONAL = 0,  // Sun/Moon - no position needed
    POINT = 1,        // Bulb/Torch - light from a point
    SPOT = 2          // Flashlight/Spotlight - cone of light
};
```

---

## Three Lighting Functions

### 1. CalcDirLight
- **When to use**: Sun, moon, large uniform light source
- **Properties needed**:
  - `direction` - direction light comes FROM
  - `ambient`, `diffuse`, `specular` colors
- **No attenuation** - same brightness everywhere
- **Fastest** - no distance calculation

### 2. CalcPointLight
- **When to use**: Torch, bulb, explosion
- **Properties needed**:
  - `position` - where light is located
  - `ambient`, `diffuse`, `specular` colors
  - `constant`, `linear`, `quadratic` - attenuation
- **Distance-based attenuation** - fades with distance
- **Moderate cost** - includes distance calculation

### 3. CalcSpotLight
- **When to use**: Flashlight, stage light, car headlight
- **Properties needed**:
  - Everything from point light +
  - `direction` - where light points
  - `cutOff` - inner cone angle (cos value)
  - `outerCutOff` - outer cone edge (cos value)
- **Cone falloff** - fades outside cone
- **Smooth edge** - soft transition between inner/outer

---

## Attenuation Presets

Copy-paste attenuation values for different ranges:

```cpp
// Very close (7 units)
light.constant = 1.0f;
light.linear = 0.7f;
light.quadratic = 1.8f;

// Close (13 units)
light.constant = 1.0f;
light.linear = 0.35f;
light.quadratic = 0.44f;

// Medium (20 units)
light.constant = 1.0f;
light.linear = 0.22f;
light.quadratic = 0.20f;

// Torch (50 units) - USE THIS ONE
light.constant = 1.0f;
light.linear = 0.09f;
light.quadratic = 0.032f;

// Large (100 units)
light.constant = 1.0f;
light.linear = 0.045f;
light.quadratic = 0.0075f;
```

---

## Spot Light Cone Angles

```cpp
// Narrow beam (12.5° inner, 17.5° outer)
light.cutOff = glm::cos(glm::radians(12.5f));
light.outerCutOff = glm::cos(glm::radians(17.5f));

// Medium beam (25° inner, 30° outer)
light.cutOff = glm::cos(glm::radians(25.0f));
light.outerCutOff = glm::cos(glm::radians(30.0f));

// Wide beam (45° inner, 50° outer)
light.cutOff = glm::cos(glm::radians(45.0f));
light.outerCutOff = glm::cos(glm::radians(50.0f));
```

---
