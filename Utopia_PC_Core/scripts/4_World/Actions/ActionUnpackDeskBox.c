class ActionUnpackDeskBox: ActionUnpackBox
{
    override void OnFinishProgressServer(ActionData action_data)
    {
        if (action_data.m_MainItem && action_data.m_MainItem.GetHierarchyRootPlayer() == action_data.m_Player)
        {
            string path = "CfgVehicles " + action_data.m_MainItem.GetType();
            array<string> itemsToSpawn = {
                "Utopia_PC_Desk_Top_1",
                "Utopia_PC_Desk_Top_2",
                "Utopia_PC_Leg_1",
                "Utopia_PC_Leg_2",
                "Utopia_PC_Leg_3",
                "Utopia_PC_Case",
                "Utopia_Desk_Manual"
            };

            vector position = action_data.m_Player.GetPosition();
            vector orientation = action_data.m_Player.GetOrientation();

            foreach (string itemType: itemsToSpawn)
            {
                EntityAI item = EntityAI.Cast(GetGame().CreateObject(itemType, position, false, false, true));
                if (item)
                {
                    item.SetPosition(position);
                    item.SetOrientation(orientation);
                }
            }

            // Delete the box after unpacking the items
            GetGame().ObjectDelete(action_data.m_MainItem);
            action_data.m_Player.GetSoftSkillsManager().AddSpecialty(m_SpecialtyWeight);
        }
    }
}
