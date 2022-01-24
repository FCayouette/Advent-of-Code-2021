open System.IO

let ProcessChunk (c:string[]) =
    let ofInterest = [|c.[4]; c.[5]; c.[15]|] |> Array.map(fun (s:string) -> s.Split(' ').[2] |> System.Int32.Parse)
    if (ofInterest.[0] = 1) then (true, ofInterest.[2]) 
    else (false, ofInterest.[1])

let GetMax (result, stack) (push, value) =
    if push then (Array.append result [|0|], Array.append stack [|value|]) else
    let delta = value + stack.[stack.Length-1]
    let newStack = stack.[0..stack.Length-2]
    if delta > 0 then 
        result.[Array.findIndexBack(fun x -> x=0) result] <- 9-delta
        (Array.append result [|9|], newStack)
    else 
        result.[Array.findIndexBack(fun x -> x=0) result] <- 9
        (Array.append result [|9+delta|], newStack)

let GetMin (result, stack) (push, value) = 
    if push then (Array.append result [|0|], Array.append stack [|value|]) else
    let delta = value + stack.[stack.Length-1]
    let newStack = stack.[0..stack.Length-2]
    if delta > 0 then 
        result.[Array.findIndexBack(fun x -> x=0) result] <- 1
        (Array.append result [|1+delta|], newStack)
    else
        result.[Array.findIndexBack(fun x -> x=0) result] <- 1-delta
        (Array.append result [|1|], newStack)


[<EntryPoint>]
let main argv =
    let blocks = File.ReadAllLines(argv.[0]) |> Array.chunkBySize 18 |> Array.map ProcessChunk
    let part1,_ = Array.fold(GetMax) ([||],[||]) blocks
    Array.fold(fun t x -> t*10L + (int64)x) 0L part1 |> printfn "Part 1: %d"
    let part2,_ = Array.fold(GetMin) ([||],[||]) blocks
    new string(Array.map(fun x-> (char)(x + (int)'0')) part2 ) |> printfn "Part 2: %s"
    0
