## Runtime Audio System (UE5 + FMOD) — Code Samples

This repository contains a focused set of C++ code samples extracted from a production Unreal Engine 5 project, demonstrating a runtime audio system designed with performance and scalability in mind.
Extended implementation available upon request.

The system is built around a central audio manager responsible for evaluating playback decisions, while audio components act solely as execution layers. Gameplay and animation systems never play audio directly — they route requests through the audio system.

The samples demonstrate distance-based LOD evaluation, batched decision-making using ParallelFor, asynchronous occlusion checks, and direct integration with FMOD runtime parameters.

Animation-driven audio triggering is shown via a minimal AnimNotify implementation, acting strictly as a gameplay trigger rather than an audio controller.

Editor-only tooling and non-essential features have been intentionally omitted to keep the code focused on runtime system design.

Author:
Igor Szymański — Sound Director / Sound Designer / Audio Programmer
UE5 · FMOD · C++ · Runtime Audio Systems