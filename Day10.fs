open System.IO

let IsOpening x = 
    x = '(' || x = '{' || x = '[' || x ='<'

let MatchingClose x =
    match x with '(' -> ')' | '{' -> '}' | '[' -> ']' | _ -> '>'

let CorruptScore x =
    match x with ')' -> 3 | ']'->57 | '}' -> 1197 | _ -> 25137

let Process (score, stack) x =
    if score > 0 then (score, stack)
    elif IsOpening x then (0, Array.append stack [|(MatchingClose x)|]) 
    elif x <> stack.[stack.Length-1] then (CorruptScore x, [||])
    else (0, stack.[0 .. stack.Length-2])

let UnwindScore x = match x with ')' -> 1L | ']'->2L | '}' -> 3L | _ -> 4L

[<EntryPoint>]
let main argv =
    let corruptIncomplete = File.ReadAllLines(argv.[0]) |> Array.map(fun (s:string)-> s.ToCharArray() |> Array.fold(Process) (0, [||]))
    printfn "Part 1 %A" <| Array.fold(fun t (x, _) -> t+x) 0 corruptIncomplete
    let tmp = Array.map(fun (_,s)-> Array.foldBack(fun x t -> t*5L + (UnwindScore x)) s 0L) corruptIncomplete |> Array.filter(fun x -> x > 0L) |> Array.sort
    printfn "Part 2 %A" tmp.[tmp.Length/2]
    0