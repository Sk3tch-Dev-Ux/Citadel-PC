modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert( ActionTurnOnCitadelPC );
		actions.Insert( ActionTurnOffCitadelPC );
        actions.Insert( ActionUnpackDeskBox);
        actions.Insert( ActionDismantleCitadel_Desk);
        actions.Insert( ActionDismantleCitadelPCDeskHologram);
        actions.Insert( ActionBuildCitadelDesk);
        actions.Insert( ActionStartMining);
        actions.Insert( ActionStopMining);
        actions.Insert( ActionWithdrawCrypto);
    }
}
