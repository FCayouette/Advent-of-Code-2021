open System.IO

let Increase = Array.map(fun b -> if b = 9y then 1y else b+1y)

let Extend x = 
    let added1 = Increase x
    let added2 = Increase added1
    let added3 = Increase added2
    let added4 = Increase added3
    Array.append(Array.append(Array.append(Array.append x added1) added2) added3) added4

let Expand x =
    let exp = Array.map(Increase)
    let added1 = exp x
    let added2 = exp added1
    let added3 = exp added2
    let added4 = exp added3
    Array.append(Array.append(Array.append(Array.append x added1) added2) added3) added4

let Dijkstra (map:sbyte[][]) start target =
    let mutable toProcess = [|(-(int)map.[fst start].[snd start], start)|] // priority queue would improve performance here
    let mutable score = 0
    while toProcess.Length > 0 && score=0 do
        let (risk, pos) = toProcess.[0]
        if pos = target then score <- risk + (int)map.[fst pos].[snd pos] else
        toProcess <- Array.sub toProcess 1 (toProcess.Length-1)
        if 0y <> map.[fst pos].[snd pos] then
            let newRisk = risk + (int)map.[fst pos].[snd pos]
            map.[fst pos].[snd pos] <- 0y
            if fst pos > 0 then toProcess <- Array.append toProcess [|(newRisk, (fst pos-1, snd pos))|]
            if snd pos > 0 then toProcess <- Array.append toProcess [|(newRisk, (fst pos, snd pos-1))|]
            if fst pos < fst target then toProcess <- Array.append toProcess [|(newRisk, (fst pos+1, snd pos))|]
            if snd pos < snd target then toProcess <- Array.append toProcess [|(newRisk, (fst pos, snd pos+1))|]
            toProcess <- Array.sortBy(fun (r, _) -> r) toProcess
    score

[<EntryPoint>]
let main argv =
    let initialMap = File.ReadAllLines(argv.[0]) |> Array.map(fun (s:string) -> s.ToCharArray() |> Array.map(fun c-> (sbyte)c - (sbyte)'0'))
    let expandedMap = Expand (Array.map(Extend) initialMap)
    printfn "Part 1: %A" <| Dijkstra initialMap (0,0) (initialMap.[0].Length-1, initialMap.Length-1)
    printfn "Part 2: %A" <| Dijkstra expandedMap (0,0) (expandedMap.[0].Length-1, expandedMap.Length-1)
    0
