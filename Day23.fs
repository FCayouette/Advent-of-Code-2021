open System.IO

let ProcessInput (data:string[]) =
    let clean = Array.map(fun (s:string) -> s.[1..s.Length-2]) data
    clean.[1..3]

let TopPos = [0;1;3;5;7;9;10]
let Targets = Map[('A', 2); ('B', 4); ('C', 6); ('D', 8)]
let Costs = Map[('A', 1); ('B', 10); ('C', 100); ('D', 1000)]

let Empty (state:string) locations = 
    None=Seq.tryFind(fun x -> state.[x]<>'.') locations

let RoomIndices x l = { 11 + x%11 .. 11 .. l }

let Delta from dest = 
    let hor = abs (from%11 - dest%11)
    let ver = abs (from/11 - dest/11)
    hor+ver

let Move oldCost from dest (state:string) =
    let cost = (Delta from dest) * Costs.[state.[from]] + oldCost
    let newState = state.ToCharArray()
    newState.[dest] <- state.[from]
    newState.[from] <- '.'
    (cost, new string(newState))

let CanLeave where (state:string) =
    let target = Targets.[state.[where]]
    let roomIndices = RoomIndices where state.Length
    let above = Seq.filter(fun x -> x<where) roomIndices
    if not (Empty state above) then false 
    elif where%11 <> target then true else
    None<>(Seq.filter(fun x -> x>where) roomIndices|>Seq.tryFind(fun x -> state.[x]<>state.[where]) )

let GetSpotsFromRoom r (state:string) = 
    let left, right = Seq.filter(fun x -> x < r) TopPos, Seq.filter(fun x-> x > r) TopPos
    let _,ls = Seq.foldBack(fun x (go, n) -> if not go || state.[x]<>'.' then (false, n) else (true, Seq.append n [x])) left (true, Seq.empty)
    let _,rs = Seq.fold(fun (go,n) x -> if not go || state.[x]<>'.' then (false, n) else (true, Seq.append n [x])) (true, Seq.empty) right
    Seq.append ls rs

let GenerateMoves cost from state =
    let targets = GetSpotsFromRoom (from%11) state 
    Seq.map(fun t -> Move cost from t state) targets |> Seq.toArray
    
let CanEnterRoom t (state:string) =
    let target = Targets.[state.[t]]
    let between = Seq.filter(fun x -> x>(min target t) && x<(max target t)) TopPos
    if None<>Seq.tryFind(fun x -> state.[x]<>'.') between then None else
    let room = RoomIndices target state.Length
    if None<>Seq.tryFind(fun x -> state.[t]<>state.[x] && state.[x]<>'.') room then None else
    Some(Seq.find(fun x -> state.[x]='.') (Seq.rev room))

let TopRow i (state:string) =
    if state.[i]='.' then None else
    let check = CanEnterRoom i state
    if check.IsNone then None else Some(i, check.Value)

let Dijkstra (start:string) target =
    let mutable seenStates = Set[]
    let mutable toEval = [|(0, start)|]
    let mutable score = 0
    let allRoomIndices = Array.fold(fun t x -> Array.append t (RoomIndices x start.Length |> Seq.toArray)) [||] [|2;4;6;8|]
    while score=0 && toEval.Length > 0 do
        let (s, curState) = toEval.[0]
        toEval <- toEval.[1..toEval.Length-1]
        if curState=target then score <- s
        elif not (seenStates.Contains curState) then
          seenStates <- seenStates.Add(curState)
          let fromTop = Seq.choose(fun x -> TopRow x curState) TopPos |> 
            Seq.map(fun (f, t) -> Move s f t curState) |> Seq.toArray
          toEval<- Array.append toEval fromTop
          let fromIndex = Array.filter(fun x -> curState.[x]<>'.') allRoomIndices|> Array.filter(fun x -> CanLeave x curState) 
          let fromRooms = Array.fold(fun t x -> Array.append t (GenerateMoves s x curState))[||] fromIndex
          toEval<-Array.append toEval fromRooms |> Array.sortBy(fun (c,_) -> c)
    score
    
[<EntryPoint>]
let main argv =
    let initState = File.ReadAllLines(argv.[0]) |> ProcessInput
    let target ="...........##A#B#C#D## #A#B#C#D"
    let part2State = Array.concat [initState.[0..1]; [|" #D#C#B#A# "; " #D#B#A#C# "|]; [|initState.[2]|] ]
    let part2Target = target + "#  #A#B#C#D#  #A#B#C#D"
    printfn "Part 1: %A" <| Dijkstra (Array.fold(fun t s->t+s) "" initState) target
    printfn "Part 2: %d" <| Dijkstra (Array.fold(fun t s->t+s) "" part2State) part2Target
    0
