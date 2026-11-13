# ProjectKillerTime

Cómo se juega:
Lobby: cada jugador elige equipo (solo puede haber un Killer) y el host inicia la partida.
Survivors: mantienen la interacción sobre los templos para purificarlos y pueden curar a compañeros heridos (Healthy → Injured → Fallen).
Killer: ataque normal o cargado (mantener más de 0.3 s) y trampas con recarga.

Cómo se gana:
Survivors:	Purificar todos los templos.
Killer:	Que se acabe el tiempo (MatchDuration, 10 min por defecto).
Al terminar, todos ven un widget con el equipo ganador y un botón para volver al menú.

Ficha Técnica de Red.

RepNotify:

CurrentTeam en AProjectKillerTimePlayerState -> OnRep_TeamSelected avisa al GameState (OnLobbyTeamsUpdated) para refrescar la lista de equipos en el lobby de todos los jugadores.
TotalTemples y ActivatedTemples en AProjectKillerTimeGameState -> actualizan el contador de templos restantes en el HUD de cada jugador (OnTemplesUpdated).
WinningTeam en AProjectKillerTimeGameState -> OnRep_WinningTeam hace que cada cliente muestre el widget de resultado final con el equipo ganador.
TempleCharge, bIsActivated y bIsCharging en ATemple -> TempleCharge actualiza la barra de progreso (OnTempleCharge); bIsActivated oculta la barra y cambia el material del templo al purificarse; bIsCharging marca que el templo está siendo cargado.
HealthState, bBeingHealed y HealProgress en UHealthComponent -> actualizan el estado visual del Survivor y la barra de curación.
CurrentTraps en UAbilitiesComponent -> actualiza el contador de trampas en el HUD del Killer.
Variable replicada sin RepNotify: MatchEndTime en AProjectKillerTimeGameState -> guarda la hora de fin de la partida. Cada cliente calcula el tiempo restante localmente con GetServerWorldTimeSeconds(), por lo que el temporizador queda sincronizado sin enviar datos cada segundo.

Server RPC:

Sever_SetTeam en AProjectKillerTimePlayerState -> el servidor rechaza la elección si ya existe otro jugador con el equipo Killer.
Server_InteractStarted y Server_InteractCompleted en ASurvivorCharacter (con WithValidation) -> el servidor verifica que el actor sea válido e implemente IInteractInterface. Cada actor valida su propio estado: el templo solo se carga en el servidor y si no está activado; el Survivor solo se cura si no está sano.
Server_DoAttack en AKillerCharacter -> el servidor ejecuta el trazado del golpe, aplica el daño y controla las velocidades de ataque.
Server_PlaceTrap en UAbilitiesComponent -> el servidor verifica que haya trampas disponibles, las descuenta, crea la trampa y arranca la recarga.
Client RPC: no se usan. Cada jugador recibe la información por variables replicadas con RepNotify (por ejemplo, el resultado final con WinningTeam), lo que además funciona para jugadores que se conectan tarde.
NetMulticast RPC: Multicast_PlayAnimAttack en AKillerCharacter (Unreliable) -> reproduce la animación de ataque del Killer en todos los clientes.

Cómo probarlo.

Abrir el mapa Lvl_Menu.
Play en Standalone Game con 2 o más jugadores.
Un jugador crea la partida (Host) y el otro se une.
Cada uno elige equipo y el host presiona Start.
Probar los dos finales: purificar todos los templos, o dejar que se acabe el tiempo (se puede reducir MatchDuration en el Blueprint del GameMode).
