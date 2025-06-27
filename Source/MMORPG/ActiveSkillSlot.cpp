
#include "ActiveSkillSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UActiveSkillSlot::InitSlot(const FString& InKeybind)
{
    if (KeybindText)
    {
        KeybindText->SetText(FText::FromString(InKeybind));
    }

    // default icon to empty or placeholder
}
