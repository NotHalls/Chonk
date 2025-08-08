# PERFORMANCE REPORT  
*This file includes all the performance test notes.*

---

## Game Specifications

### Chunk Config:
**Chunk Size (1 Chunk):**
- **X:** 16  
- **Y:** 16  
- **Z:** 16  

### Settings
- **VSync**: OFF

## System Specifications

**Device:** Acer Nitro 5 (Nitro AN515-45)

### CPU (In Use)
- **Model:** AMD Ryzen 5 5600H with Radeon Graphics  
- **Clock Speed:** 3.30 GHz  
- **Cores:** 6  
- **Threads:** 12  

### Dedicated GPU (In Use)
- **Model:** NVIDIA GeForce GTX 1650  
- **VRAM:** 4.0 GB  

### Integrated GPU
- **Model:** AMD Radeon  
- **VRAM:** 512 MB  

### RAM
- **Total Memory:** 8 GB  
- **Speed:** 3200 MHz  
- **Type:** DDR4  

---

# 1. No Optimizations (Release Mode)
### ***⚠️ For This Test*** **VSync** ***Was*** **ON**

### Metrics
- **Average FPS:** 135  
- **Average Frame Time:** 7.1 ms  

### Remarks:  
- Sometimes the FPS drops to an AVG of 75 and FrameTime goes up to 12ms

---

# 2. Chunking Optimization (Release Mode)  
> Note that I also added a `Scene` class in which I have a `chunks` vector that contains all the chunks that need to be drawn on the screen.  
> I also added some other modularization like `processes`, `windowing`, and `initialization`, etc.  
> Meaning, CPU time may increase.

### Metrics
- **Average FPS:** 1050
- **Average Frame Time:** 1.19 ms  

### Remarks:  
1. The GPU usage spikes to **98% to 100%** a lot of times for some **fucking reason**.  
   - This didn’t happen previously — only after this optimization. 
   - This happens in both **Debug** and **Release** modes.  

2. I noticed some **Spike Downs** in FPS too (**10 to 50 FPS**) and **Spike Ups** in FrameTime (**25ms to 30ms**).  
   - This happens **more** in Debug Mode.

---

# 3. Face Culling Optimization (Release Mode)
> This Optimization note is made after adding several other stuff like infinet world generation.
> Also, there are <br>
> There are two tests for this optimization, first with only One Chunk (Render Distance: 0). and another with 441 Chunks (Render Distance: 10). <br>
> This is because Chunk face culling cant be tested with just One Chunk
<br>

> There are `Not Moving` and `Moving` versions of the test. <br>
> This is because the calculations for regenerating a Chunk is only done when the Camera is `Moving` and not Spectating (Spectating moves the camera but stops the Chunk Regeneration process).
<br>

> Some other notable Things are: <br>
> **Camera Speed:** *10*

## Chunk Size: `X: 16; Y: 16; Z: 16` With Render Distance of `0` `(1 Chunk)`:
## ***Not Moving***
### Metrics
- **Average FPS:** 1050
- **Average Frame Time:** 1.05 ms
### Moving:
- **Average FPS:** 1050
- **Average Frame Time:** 1.13 ms

## Chunk Size: `X: 16; Y: 16; Z: 16` With Render Distance Of `10` `(441 Chunks)`:
### Not Moving
- **Average FPS:** 900
- **Average Frame Time:** 1.25 ms
### Moving
- **Average FPS:** 870
- **Average Frame Time:** 1.29 ms