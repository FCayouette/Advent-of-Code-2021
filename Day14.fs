open System.IO

let GenerateTable (inst:string[]) =
    Array.map(fun (s:string)->s.Replace(" -> ", "")) inst |> Array.fold(fun (table:Map<(char*char), char>) s -> table.Add((s.[0],s.[1]), s.[2])) (Map[])

let InitialCount (init:string) =
    let cArray = init.ToCharArray()
    let counts = Array.countBy(id) cArray |> Array.fold(fun (map:Map<char, int64>) (x,y)-> map.Add(x,(int64)y)) (Map[])
    let pairs = Array.pairwise cArray |> Array.countBy(id) |> Array.fold(fun (map:Map<(char * char), int64>) (x,count) -> map.Add(x, (int64)count)) (Map[])
    (counts, pairs)

let AddToMap (map:Map<'a, 'b>) (a, count) = 
    if None <> map.TryFind(a) then map.Add(a, count + map.[a]) else map.Add(a, count)

let ProcessPair ((counts:Map<'a, 'b>), (pairs:Map<'a*'a, 'b>), (table:Map<'a*'a, 'a>)) pair delta =
    let c = table.[pair]
    let newCounts = AddToMap counts (c, delta)
    let newPairs = AddToMap (AddToMap pairs ((fst pair, c), delta)) ((c, snd pair), delta)
    (newCounts, newPairs, table)

let Polymerize ((counts:Map<'a, 'b>), (pairs:Map<'a*'a, 'b>)) table =
    let (newC, newP, newT) = Map.fold(ProcessPair) (counts, (Map[]), table) pairs
    (newC, newP)

let Count (map:Map<'a, int64>) =
    let (min, max) = Map.fold(fun (m, M) _ c -> (min m c, max M c)) (System.Int64.MaxValue,0L) map
    max - min

[<EntryPoint>]
let main argv =
    let raw = File.ReadAllLines(argv.[0])
    let start = InitialCount raw.[0]
    let table = Array.filter(fun (s:string) -> s.Contains("->")) raw |> GenerateTable
    let (part1, _) = Array.fold(fun t _->Polymerize t table) start [|1..10|]
    printfn "Part 1: %A" <| Count part1
    let (part2, _) = Array.fold(fun t _ -> Polymerize t table) start [|1..40|]
    printfn "Part 2: %A" <| Count part2
    0
