open System.IO

let Parse = Array.map(fun (s:string)-> s.Split(',') |> Array.map System.Int32.Parse)

let rec GenerateData raw =
    let check = Array.tryFindIndex(fun (s:string) -> s.Contains("---")) raw
    if None=check then [|(Parse raw)|] else 
    let cur, next = Array.splitAt check.Value raw
    Array.append [|Parse cur|] (GenerateData next.[1..next.Length-1])

let MakeMatrix (i:'a[]) (j:'a[]) =
    let k = [|i.[1]*j.[2]-i.[2]*j.[1]; -i.[0]*j.[2]+i.[2]*j.[0]; i.[0]*j.[1]-i.[1]*j.[0]|]
    Array.concat [[|i|];[|j|];[|k|]]

let GenerateDistances beacons = 
    let FindAllDistances (b:int[][]) =
        let mutable distances = Map[||]
        for i in [0..b.Length-2] do
            for j in [i+1..b.Length-1] do
                let d = Array.fold2(fun t a b -> t + (a-b)*(a-b)) 0 b.[i] b.[j]
                distances <- distances.Add(d, (i,j))
        distances
    Array.map(FindAllDistances) beacons

let DistCheck (solved:Map<'a,'b>) (points:Map<'a,'b>) =
    let counts = Map.toSeq points |> Seq.filter(fun (k,_)-> None<>Map.tryFind k solved)
    if Seq.length counts < 55 then None else
    let lastMatch = Seq.last counts
    Some(solved.[fst lastMatch], snd lastMatch)

let Transform (m:int[][]) (v:int[]) =
    let result = [|0;0;0|]
    for i in [0..2] do
        result.[i] <- (Array.map2(*) m.[i] v |> Array.sum)
    result

[<EntryPoint>]
let main argv =
    let raw = File.ReadAllLines(argv.[0]) |> Array.filter(fun (s:string) -> s.Length>0)
    let beacons = GenerateData raw.[1..raw.Length-1]
    let distances = GenerateDistances beacons
    let mutable solvedIndices, toSolve = [|0|], set[1..beacons.Length-1]
    let mutable positions, maxManhattan = [|[|0;0;0|]|], 0
    let Distance = Array.map2(fun x y -> abs(x-y))
    let Add = Array.map2(fun x y -> x+y)
    
    let Solve sIndex pIndex ((si0, si1), (pi0, pi1))=
        let s0, s1 = beacons.[sIndex].[si0], beacons.[sIndex].[si1]
        let p0, p1 = beacons.[pIndex].[pi0], beacons.[pIndex].[pi1]
        let s, p = Distance s0 s1, Distance p0 p1
        let dir0 = Array.map(fun x -> if x=s.[0] then 1 else 0) p
        let dir1 = Array.map(fun x -> if x=s.[1] then 1 else 0) p
        let mutable found = false
        for t in [0..3] do 
          if not found then
            if t=2 then Array.iteri(fun i v -> dir0.[i] <- -v) dir0 
            elif t%2=1 then Array.iteri(fun i v -> dir1.[i] <- -v) dir1 
            let transform = MakeMatrix dir0 dir1
            let t = Array.map(fun x -> Transform transform x) [|p0; p1|]
            Array.iteri(fun i x -> if not found then 
                                     let scannerPos = Array.map2(fun a b -> a-b) s0 x
                                     let testPos = Add scannerPos t.[(i+1)%2]
                                     let check = Array.tryFind(fun x -> x=testPos) beacons.[sIndex]
                                     if None<>check then
                                       found <- true
                                       maxManhattan <- max maxManhattan (Array.map(fun x -> Distance x scannerPos |> Array.sum) positions |> Array.max)
                                       positions <- Array.append positions [|scannerPos|]
                                       beacons.[pIndex] <- Array.map(fun x -> Transform transform x |> Add scannerPos) beacons.[pIndex]) t

    let TrySolve index =
        let mutable found = false
        Array.iter(fun solvedIndex -> if not found then
                                      let check = DistCheck distances.[solvedIndex] distances.[index]
                                      if None<>check then 
                                        found <- true
                                        Solve solvedIndex index check.Value
                                        toSolve <- toSolve.Remove(index)
                                        solvedIndices <- Array.append solvedIndices [|index|]) solvedIndices

    while toSolve.Count > 0 do
        Array.iter(TrySolve) <| Set.toArray toSolve
    
    Array.fold(fun s x -> Array.append s x) [||] beacons |> Array.distinct |> Array.length |> printfn "Part 1: %d"
    printfn "Part 2: %d" maxManhattan
    0
