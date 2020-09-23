/*
 * retroreflector
 *   Copyright (C) 2020 Benjamin Jackson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bzfsAPI.h"

class retroreflector : public bz_Plugin
{
public:
    virtual const char* Name();
    virtual void Init(const char* config);
    virtual void Cleanup();
    virtual void Event(bz_EventData* eventData);
};

BZ_PLUGIN(retroreflector)

const char* retroreflector::Name()
{
    return "retroreflector 1.0.1";
}

void retroreflector::Init(const char* config)
{
    Register(bz_eFlagGrabbedEvent);
    Register(bz_ePlayerDieEvent);

    bz_RegisterCustomFlag("RR", "Retrorreflector", "Reflects laser back to shooter", 0, eGoodFlag);
}

void retroreflector::Cleanup()
{
    Flush();
}

void retroreflector::Event(bz_EventData* eventData)
{
    switch (eventData->eventType)
    {
    case bz_ePlayerDieEvent:
    {
        // This event is called each time a tank is killed.
        bz_PlayerDieEventData_V2* diedata = (bz_PlayerDieEventData_V2*)eventData;

        bz_BasePlayerRecord* killer = bz_getPlayerByIndex(diedata->killerID);

        if (killer)
        {
            if (diedata->flagKilledWith == "L" && killer->spawned)
            {
                std::string RR = bz_getFlagName(diedata->flagHeldWhenKilled);
                if ("RR" == RR)
                {
                    bz_sendTextMessagef(BZ_SERVER, diedata->killerID, "You killed yourself by shooting %s while they held retroreflector", bz_getPlayerCallsign(diedata->playerID));
                    bz_killPlayer(diedata->killerID, false, diedata->playerID, "RR");
                    bz_incrementPlayerLosses(diedata->playerID, -1);
                }
            }
        }
        bz_freePlayerRecord(killer);

        // Data
        // ----
        // (int)                  playerID           - ID of the player who was killed.
        // (bz_eTeamType)         team               - The team the killed player was on.
        // (int)                  killerID           - The owner of the shot that killed the player, or BZ_SERVER for server side kills
        // (bz_eTeamType)         killerTeam         - The team the owner of the shot was on.
        // (bz_ApiString)         flagKilledWith     - The flag name the owner of the shot had when the shot was fired.
        // (int)                  flagHeldWhenKilled - The ID of the flag the victim was holding when they died.
        // (int)                  shotID             - The shot ID that killed the player, if the player was not killed by a shot, the id will be -1.
        // (bz_PlayerUpdateState) state              - The state record for the killed player at the time of the event
        // (double)               eventTime          - Time of the event on the server.
    }
    break;

    default:
        break;
    }
}
