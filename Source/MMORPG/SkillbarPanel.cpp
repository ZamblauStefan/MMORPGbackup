
#include "SkillbarPanel.h"
#include "ActiveSkillSlot.h"

void USkillbarPanel::InitSkillbar()
{
    if (Slot1)
        Slot1->InitSlot("1");
    if (Slot2)
        Slot2->InitSlot("2");
    if (Slot3)
        Slot3->InitSlot("3");
    if (Slot4)
        Slot4->InitSlot("4");
}

