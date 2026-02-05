// Author: Igor Szymański
// Portfolio code sample
// Public implementation excerpt. Full implementation available upon request.

// Managed runtime audio component.
// Registers itself with the central audio manager and executes
// playback, occlusion and lifecycle control based on system decisions.

void UManagedAudioComponent::RegisterWithAudioSystem()
{
    if (bCheckOcclusion)
        AudioManager->RegisterAudioSource(this, MinLOD, MaxLOD, FAudioLODSourceData::FAudioSourceOcclusionControl::CreateUObject(this, &UManagedAudioComponent::SetOccluded), FAudioLODSourceData::FAudioSourcePlaybackControl::CreateUObject(this, &UManagedAudioComponent::Play), FAudioLODSourceData::FAudioSourcePlaybackControl::CreateUObject(this, &UManagedAudioComponent::Stop));
    else
        AudioManager->RegisterAudioSource(this, MinLOD, MaxLOD, FAudioLODSourceData::FAudioSourcePlaybackControl::CreateUObject(this, &UManagedAudioComponent::Play), FAudioLODSourceData::FAudioSourcePlaybackControl::CreateUObject(this, &UManagedAudioComponent::Stop));
    bRegisteredWithAudioSystem = true;
}

void UManagedAudioComponent::SetEnabled(bool bEnabled)
{
    if (bEnabled) {
        if (bIsEnabled)
            return;

        bIsEnabled = true;
        if (AudioManager.IsValid())
            RegisterWithAudioSystem();
    }
    else {
        if (!bIsEnabled)
            return;
        bIsEnabled = false;
        if (AudioManager.IsValid())
            UnregisterFromAudioSystem();
    }
}
