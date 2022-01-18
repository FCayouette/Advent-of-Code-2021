open System.IO

let ExtractPos (s:string) =
    let data = s.Replace(": ", ":").Split(':')
    (System.Int32.Parse data.[1]) - 1

let rec Part1 (p1, s1) (p2, s2) dice rolls =
    if s2 >= 1000 then s1*rolls else
    let np1 = (p1 + 3*(dice+1)) % 10
    Part1 (p2, s2) (np1, s1+np1+1) ((dice + 3) % 10) (rolls+3)

let AddToMap (map:Map<'a,'b>) (k,v) =
    let check = map.TryFind(k)
    if None=check then map.Add(k,v) else map.Add(k, v+check.Value)    

[<EntryPoint>]
let main argv =
    let pos = File.ReadAllLines(argv.[0]) |> Array.map(ExtractPos)
    printfn "Part 1: %d" (Part1 (pos.[0], 0) (pos.[1], 0) 1 0)
    let distribution = [|(3,1L); (4,3L); (5,6L); (6,7L); (7,6L); (8,3L); (9,1L)|]
    let mutable win1, win2, firstPlayer = 0L, 0L, true
    let mutable states = Map[(pos.[0], 0, pos.[1], 0), 1L]

    let Part2 state = 
        let mutable next = Map[]
        Map.iter(fun (p0, s0, p1, s1) count -> 
             if firstPlayer then
                 Array.iter(fun (delta, c) -> 
                     let np = (delta + p0)%10
                     let ns = s0 + np + 1
                     if ns >= 21 then win1 <- win1 + c*count else
                         next <- AddToMap next ((np,ns,p1,s1),(c*count))) distribution
             else Array.iter(fun (delta, c) ->
                     let np = (delta + p1)%10
                     let ns = s1 + np + 1
                     if ns >= 21 then win2 <- win2 + c*count else
                     next <- AddToMap next ((p0, s0, np, ns),(c*count))) distribution ) state
        states <- next
        firstPlayer <- not firstPlayer

    while states.Count > 0 do 
        Part2 states
    printfn "Part 2: %d" (max win1 win2)
    0
