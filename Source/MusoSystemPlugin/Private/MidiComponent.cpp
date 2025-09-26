#include "MidiComponent.h"
#include "MusoSubsystem.h"


UMidiComponent::UMidiComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMidiComponent::BeginPlay()
{
	Super::BeginPlay();

	UMusoSubsystem* MusoSubsystem = GetWorld()->GetSubsystem<UMusoSubsystem>();
	if (MusoSubsystem) {
		//MusoSubsystem->MidiPlayer->OnNextMidiEvent.AddDynamic(this, &UMidiComponent::OnNextMidiEvent);
		UE_LOG(LogTemp, Error, TEXT("OnMidiEvent subscribed"));
	}
}

void UMidiComponent::InitializeComponent()
{
	Super::InitializeComponent();


}


void UMidiComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMidiComponent::OnNextMidiEvent(int32 Ticks)
{
	UE_LOG(LogTemp, Warning, TEXT("OnMidiEvent: %d"), Ticks);
}

void UMidiComponent::SetMidiFile(UMidiFile* MidiFile)
{
	//UMusoSubsystem* MusoSubsystem = GetWorld()->GetSubsystem<UMusoSubsystem>();
	//MusoSubsystem->SetMidiFile(MidiFile);
}


