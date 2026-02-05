// Author: Igor Szymański
// Portfolio code sample
// Public implementation excerpt. Full implementation available upon request.

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<bool> CGAudioManaged_AudioSpawnDebugDraw(TEXT("AudioManager.Play.DebugDraw"), false,  TEXT("Draw debug sphere on SpawnManagedAudioEvent"), ECVF_Default);
#endif

void UAudioManager::Update()
{
	SCOPE_CYCLE_COUNTER(STAT_AudioLODManager_Update);
	{
		SCOPE_CYCLE_COUNTER(STAT_AudioLODManager_SourceUpdate);

		AudioSourcesDataCopy = AudioSourcesData;
		FCriticalSection TaskArrayMutex;

		TArray<TPair<FAudioLODSourceData*, EManageAudioSourceTask>> TaskArray;
		ParallelFor(AudioSourcesDataCopy.Num(),
			[&](int32 Index)
			{
				FAudioLODSourceData* ObjectInfo = AudioSourcesDataCopy[Index];

				check(ObjectInfo->IsValid()); 
				uint8 CurrentLOD = CalculateCurrentLOD(ObjectInfo);
				
				EManageAudioSourceTask Task = ManageAudioSourcePlayback(ObjectInfo, CurrentLOD);
				if (Task != EManageAudioSourceTask::NONE) {
					FScopeLock Lock(&TaskArrayMutex);
					TaskArray.Add({ ObjectInfo, Task });
				}
			});
			
		for (auto& Task : TaskArray) {
			ExecAudioSourcePlaybackManagement(Task.Key, Task.Value);
		}
	}
}

void UAudioManager::RegisterAudioSource(USceneComponent* AudioSource, uint8 MinLOD, uint8 MaxLOD, FAudioLODSourceData::FAudioSourceOcclusionControl _OcclusionControl, FAudioLODSourceData::FAudioSourceUpdate _Update)
{
	FAudioLODSourceData* AudioLODSourceData = new FAudioLODSourceData(AudioSource, MinLOD, MaxLOD);
	AudioLODSourceData->BindUpdateDelegate(_Update);
	AudioLODSourceData->BindOcclusionControlDelegate(_OcclusionControl);
	RegisterManagedAudioSource(AudioLODSourceData);
}

void UAudioManager::UnregisterAudioSource(USceneComponent* AudioSource)
{
	DEC_DWORD_STAT(STAT_AudioLODManager_NumObjects);
	SCOPE_CYCLE_COUNTER(STAT_AudioLODManager_UnregisterObject);

	FAudioLODSourceData* ObjectInfo;
	if (AudioSources.RemoveAndCopyValue(AudioSource, ObjectInfo))
	{
		AudioSourcesData.RemoveSwap(ObjectInfo, EAllowShrinking::No);
		delete ObjectInfo;
	}
}

void UAudioManager::K2_PlaySimpleManagedAudioEventOneShot(UObject* WorldObjectContext, UFMODEvent* Event, const FVector Location, bool bCheckOcclusion, bool bSpawnWhenOccluted)
{
	AActor* OwningActor = Cast<AActor>(WorldObjectContext);

	FTransform Transform(Location);
	PlayManagedAudioEventOneShot(Event, Transform, TMap<FName, float>(), OwningActor, bCheckOcclusion, bSpawnWhenOccluted);
}
