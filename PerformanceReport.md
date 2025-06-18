# 🚀 PERFORMANCE REPORT  
*This file includes all the performance test notes.*

---

## 📦 Chunk Configuration

**Chunk Size (1 Chunk):**
- **X:** 16  
- **Y:** 16  
- **Z:** 16  

## 💻 System Specifications

**Device:** Acer Nitro 5 (Nitro AN515-45)

### 🧠 CPU (In Use)
- **Model:** AMD Ryzen 5 5600H with Radeon Graphics  
- **Clock Speed:** 3.30 GHz  
- **Cores:** 6  
- **Threads:** 12  

### 🎮 Dedicated GPU (In Use)
- **Model:** NVIDIA GeForce GTX 1650  
- **VRAM:** 4.0 GB  

### 🖥️ Integrated GPU
- **Model:** AMD Radeon  
- **VRAM:** 512 MB  

### 💾 RAM
- **Total Memory:** 8 GB  
- **Speed:** 3200 MHz  
- **Type:** DDR4  

---

# ➡️ No Optimizations (Release Mode)

### 📊 Metrics
- **Average FPS:** 135  
- **Average Frame Time:** 7.1 ms  
- **Average CPU Usage:** 6.3%  
- **Average Memory Usage:** 64.6 MB  
- **Average GPU Usage:** 16%  

### ⚠️ Remarks:  
- Sometimes the FPS drops to an AVG of 75 and FrameTime goes up to 12ms

---

# ➡️ Chunking Optimization (Release Mode)  
> Note that I also added a `Scene` class in which I have a `chunks` vector that contains all the chunks that need to be drawn on the screen.  
> I also added some other modularization like `processes`, `windowing`, and `initialization`, etc.  
> Meaning, CPU time may increase.

### 📊 Metrics
- **Average FPS:** 145  
- **Average Frame Time:** 6.7 ms  
- **Average CPU Usage:** 3.0%  
- **Average Memory Usage:** 36.0 MB  
- **Average GPU Usage:** 15%  

### ⚠️ Remarks:  
1. The GPU usage spikes to **98% to 100%** a lot of times for some **fucking reason**.  
   - This didn’t happen previously — only after this optimization. 
   - This happens in both **Debug** and **Release** modes.  

2. I noticed some **Spike Downs** in FPS too (**10 to 50 FPS**) and **Spike Ups** in FrameTime (**25ms to 30ms**).  
   - This happens **more** in Debug Mode.

---
