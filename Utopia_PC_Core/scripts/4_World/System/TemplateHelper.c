//this class contains all useful methods that can be required for a project purpose.
class UtopiaPcHelper
{
    // Returns the attachment slot name for a graphics card by 1-based index (1..3)
    static string GetGraphicCardSlotName(int index)
    {
        return "Graphics_Card_" + index.ToString();
    }

    // Returns the attachment slot name for a RAM stick by 1-based index (1..4)
    static string GetRamSlotName(int index)
    {
        return "Ram_" + index.ToString();
    }

    // Returns the attachment slot name for a PSU by 1-based index (1..2)
    static string GetPSUSlotName(int index)
    {
        return "PSU_" + index.ToString();
    }

    // Returns the attachment slot name for a Kingston SSD by 1-based index (1..3)
    static string GetKingstonSlotName(int index)
    {
        return "Kingston_" + index.ToString();
    }

    // Returns the attachment slot name for a fan by 1-based index (1..2)
    static string GetFanSlotName(int index)
    {
        return "Fan_" + index.ToString();
    }

    // Returns health of an item as a 0.0-100.0 float, or -1 if item is null
    static float GetItemHealthPercent(ItemBase item)
    {
        if (!item)
            return -1;

        return item.GetHealth("", "Health");
    }

    // Returns true if the item is below the given health threshold (0.0-100.0)
    static bool IsItemDamaged(ItemBase item, float threshold = 50.0)
    {
        if (!item)
            return false;

        return GetItemHealthPercent(item) < threshold;
    }

    // Counts how many attachments are present in a contiguous numbered slot range
    static int CountNumberedAttachments(EntityAI entity, string prefix, int maxSlots)
    {
        int count = 0;
        for (int i = 1; i <= maxSlots; i++)
        {
            if (entity.GetInventory().FindAttachmentByName(prefix + i.ToString()))
                count++;
        }
        return count;
    }
}