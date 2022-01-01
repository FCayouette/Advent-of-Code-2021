open System.IO

let RawToConnections (x:string) =
    let tmp = x.Split('-')
    (tmp.[0], tmp.[1])

let AddConnection x y (connections:Map<string, string[]>) =
    if connections.ContainsKey(x) then 
        let prev = connections.[x]
        connections.Add(x, Array.append prev [|y|])
    else connections.Add(x, [|y|])

let NotBigCave (x:string) = x.[0] < 'A' || x.[0] > 'Z'

let rec CountPaths x (connections:Map<string, string[]>) path =
    if x = "end" then 1
    elif (NotBigCave x) && (None <> Array.tryFind(fun s -> s=x) path) then 0 else
    let newPath = Array.append path [|x|]
    Array.fold(fun t p -> t + CountPaths p connections newPath) 0 (connections.[x])

let rec CountPaths2 x (connections:Map<string, string[]>) path visitedTwice = 
    if x = "end" then 1 else
    let double = (NotBigCave x) && (None <> Array.tryFind(fun s -> s=x) path)
    if double && (visitedTwice || x="start") then 0 else   
    let newPath = Array.append path [|x|]
    Array.fold(fun t p -> t + CountPaths2 p connections newPath (visitedTwice || double)) 0 (connections.[x])

[<EntryPoint>]
let main argv =
    let connections = File.ReadAllLines(argv.[0]) |> Array.map(RawToConnections) |> Array.fold(fun c (x, y) -> AddConnection x y (AddConnection y x c)) (Map [])
    CountPaths "start" connections [||] |> printfn "Part 1 %A"
    CountPaths2 "start" connections [||] false |> printfn "Part 2 %A"
    0
