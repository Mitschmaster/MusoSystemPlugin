#include "MusoData.h"
#include "Misc/DataValidation.h"


EDataValidationResult UMusoData::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = EDataValidationResult::Valid;
	if (AudioTracks.Num() == 0)
	{
		Context.AddError(FText::FromString(TEXT("No audio tracks defined.")));
		Result = EDataValidationResult::Invalid;
	}
	for (const FMusoAudioParam& Track : AudioTracks)
	{
		if (Track.SoundWave == nullptr)
		{
			Context.AddError(FText::Format(FText::FromString(TEXT("Audio track '{0}' has no SoundWave assigned.")), FText::FromName(Track.Name)));
			Result = EDataValidationResult::Invalid;
		}
		if (Track.Name.IsNone())
		{
			Context.AddError(FText::FromString(TEXT("An audio track has no name assigned.")));
			Result = EDataValidationResult::Invalid;
		}
	}
	if (MidiFile == nullptr)
	{
		Context.AddError(FText::FromString(TEXT("No MIDI track assigned.")));
		Result = EDataValidationResult::Invalid;
	}
	return Result;
}

void UMusoData::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

    const FName headPropertyName = PropertyChangedEvent.PropertyChain.GetHead()->GetValue()->GetFName();
	const FName tailPropertyName = PropertyChangedEvent.GetPropertyName();

	if (headPropertyName == tailPropertyName)
	{
		if (headPropertyName == GET_MEMBER_NAME_CHECKED(UMusoData, MidiFile) && MidiFile)
			MidiTrackLength = MidiFile->GetSongMaps()->GetSongLengthMs() / 1000.0f;
	}
    else if (headPropertyName == GET_MEMBER_NAME_CHECKED(UMusoData, AudioTracks))
    {
        if (PropertyChangedEvent.ChangeType != EPropertyChangeType::ArrayAdd &&
        	PropertyChangedEvent.ChangeType != EPropertyChangeType::ArrayRemove)
        {
            const int32 changedIndex = PropertyChangedEvent.GetArrayIndex(headPropertyName.ToString());
        	if (changedIndex != INDEX_NONE)
                if ((tailPropertyName == GET_MEMBER_NAME_CHECKED(FMusoAudioParam, SoundWave) ||
                	tailPropertyName == GET_MEMBER_NAME_CHECKED(FMusoAudioParam, Pitch)) &&
                	AudioTracks[changedIndex].SoundWave)
                		AudioTracks[changedIndex].AudioTrackLength =
                			AudioTracks[changedIndex].SoundWave->Duration / AudioTracks[changedIndex].Pitch;
        }
    }
}
