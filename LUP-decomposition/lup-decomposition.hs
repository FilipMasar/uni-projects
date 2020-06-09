import Data.List
import Text.Printf


matrixMul :: Num a => [[a]] -> [[a]] -> [[a]]
matrixMul a b = [[sum (zipWith (*) a_row b_column) | b_column <- (transpose b)] | a_row <- a]

matrixVectorMul :: Num a => [[a]] -> [a] -> [a]
matrixVectorMul a v = [sum (zipWith (*) a_row v) | a_row <- a]

vectorVectorMul :: Num a => [a] -> [a] -> [[a]]
vectorVectorMul v w = [[v_elem * w_elem | w_elem <- w] | v_elem <- v]

vectorScalarMul :: Num a => [a] -> a -> [a]
vectorScalarMul v s = map (*s) v

dotProduct :: Num a => [a] -> [a] -> a
dotProduct v w = sum (zipWith (*) v w)

vectorVectorSub :: Num a => [a] -> [a] -> [a]
vectorVectorSub v w = zipWith (-) v w

matrixAdd :: Num a => [[a]] -> [[a]] -> [[a]]
matrixAdd a b = zipWith (zipWith (+)) a b

matrixSub :: Num a => [[a]] -> [[a]] -> [[a]]
matrixSub a b = zipWith (zipWith (-)) a b

matrixScalarDiv :: Fractional a => [[a]] -> a -> [[a]]
matrixScalarDiv a s = map (map (/s)) a

identyMatrix :: Num a => Int -> [[a]]
identyMatrix n = [ [if (i==j) then 1 else 0 | j <- [1..n]] | i <- [1..n]]

-- ===================================================================
-- | Matrix LUP decomposition
--   The result for a matrix A is given as (L,U,P) where:
--
--   * L is an unit lower triangular matrix.
--   * U is an upper triangular matrix.
--   * P is a permutation matrix.
--
--   A has to be squared non-singular matrix. Then PA = LU
-- ===================================================================
lup :: (Ord a, Fractional a) => [[a]] -> ([[a]], [[a]], [[a]])
lup [[a]] = ([[1]], [[a]], [[1]]) 
lup a = 
  let n = length a
      -- swap first row with the k-th row
      -- a[k,0] has the greatest absolute value among the first column.
      maxIdx = getMaxIndex (map head a)
      q = if maxIdx == 0 then identyMatrix n else swapRow maxIdx (identyMatrix n)

      newMatrix = matrixMul q a
      ak1 = head (head newMatrix)
      v = tail (map head newMatrix)
      w = tail (head newMatrix)
      a' = map tail (tail newMatrix)
     
      -- recursive call
      a_recurse = matrixSub a' (matrixScalarDiv (vectorVectorMul v w) ak1)
      (l', u', p') = lup a_recurse
      
      -- compute p
      tmpP_row = 1 : (replicate (n-1) 0)
      tmpP = tmpP_row : [0:row | row <- p']
      p = matrixMul tmpP q
      -- compute l
      tmp_col = [x / ak1 | x <- v]
      tmpL_row = 1 : (replicate (n-1) 0)
      tmpL_col = matrixVectorMul p' tmp_col
      l = tmpL_row : (map (\(x,y) -> x:y) (zip tmpL_col l'))
      -- compute u
      tmpU_row = head newMatrix
      u = tmpU_row : [0:row | row <- u']

  in (l,u,p)

getMaxIndex :: (Ord a, Num a) => [a] -> Int
getMaxIndex values = 
  let tmp = zip (map abs values) [0..]
  in snd (maximum tmp)

swapRow :: Int -> [a] -> [a]
swapRow j xs =
  let elemI = head xs
      elemJ = xs !! j
      middle = take (j - 1) (drop 1 xs)
      right = drop (j + 1) xs
  in [elemJ] ++ middle ++ [elemI] ++ right


-- ===================================================================
-- | Solve system Ax = b
--
--   * A - n*n non-singular matrix
--   * b - vector of size n
--
--   return x - solution to Ax = b 
-- ===================================================================
solve :: (Ord a, Fractional a) => [[a]] -> [a] -> [a]
solve a b = solve' (lup a) b

solve' :: Fractional a => ([[a]], [[a]], [[a]]) -> [a] -> [a]
solve' (l,u,p) b =
  let y = forwardSubstitution l (matrixVectorMul p b)
      x = backwardSubstitution u y
  in x

forwardSubstitution :: Num a => [[a]] -> [a] -> [a]
forwardSubstitution [[_]] [b] = [b]
forwardSubstitution l b =
  let y1 = head b
      l1 = tail (map head l)
      b' = vectorVectorSub (tail b) (vectorScalarMul l1 y1)
      y = forwardSubstitution (map tail (tail l)) b'
  in y1:y

backwardSubstitution :: Fractional a => [[a]] -> [a] -> [a]
backwardSubstitution [[u]] [y] = [y/u]
backwardSubstitution u y =
  let xn = (last y) / (last (last u))
      un = map last (init u)
      y' = vectorVectorSub (init y) (vectorScalarMul un xn)
      x = backwardSubstitution (map init (init u)) y'
  in x++[xn]

-- ===================================================================
-- | Computing a matrix inverse from LUP decomposition 
--
--   * A - n*n non-singular matrix
--  
--   solving set of n distinct distinct equations Ax = b.
--   Precisely Ax[i] = e[i]
--     x[i] - ith column of X
--     e[i] - ith column of Identity matrix
--   
--   return X - inverse of A
-- ===================================================================
inverse :: (Fractional a, Ord a) => [[a]] -> [[a]]
inverse a = 
  let i = identyMatrix (length a)
      x = [solve' (lup a) ei | ei <- i]
  in transpose x


-- ===================================================================
-- | Computing a determinant of matrix A from LUP decomposition 
--
--   * A - n*n non-singular matrix
--  
--   det(A) = det(inv(P)) * det(L) * det(U)
--
--   determinant of triangular matrix is product of its diagoal
--   determinant of permutational matrix is number of row exchanges
-- ===================================================================
determinant :: RealFrac a => [[a]] -> a
determinant a =
  let (l,u,p) = lup a
      detL = 1
      detU = triangularDet u
      exchanges = rowExchanges p
      detPinv = if (exchanges `mod` 2) == 0 then 1 else -1
  in detPinv * detL * detU

triangularDet :: Num a => [[a]] -> a
triangularDet [[a]] = a
triangularDet a = (head (head a)) * triangularDet (map tail (tail a))

rowExchanges :: (Ord a, Num a, Num p) => [[a]] -> p
rowExchanges [[p]] = 0
rowExchanges p =
  let maxIdx = getMaxIndex (map head p)
      p' = if maxIdx == 0 then (map tail (tail p)) else (map tail (tail (swapRow maxIdx p)))
      res = if maxIdx == 0 then 0 else 1
  in res + (rowExchanges p')


-- ===================================================================
-- | TESTING
-- ===================================================================

matrixToString m = concat $ intersperse "\n"
  (map (\x -> unwords $ printf "%8.4f" <$> (x::[Double])) 
    (map (\x -> map fromRational x) m))

lupTest :: [[Rational]] -> IO ()
lupTest a =
  let (l,u,p) = lup a
  in
    putStrLn ("A: \n" ++ matrixToString a) >>
    putStrLn ("L: \n" ++ matrixToString l) >>
    putStrLn ("U: \n" ++ matrixToString u) >>
    putStrLn ("P: \n" ++ matrixToString p) >>
    putStrLn ("Verify: PA\n" ++ matrixToString (matrixMul p a)) >>
    putStrLn ("Verify: LU\n" ++ matrixToString (matrixMul l u))

solveTest :: [[Rational]] -> [Rational] -> IO ()
solveTest a b =
  let x = solve a b
  in
    putStrLn ("A: \n" ++ matrixToString a) >>
    putStrLn ("x: \n" ++ matrixToString [x]) >>
    putStrLn ("Verify: Ax\n" ++ matrixToString ([matrixVectorMul a x])) >>
    putStrLn ("Verify: b\n" ++ matrixToString [b])

inverseTest :: [[Rational]] -> IO ()
inverseTest a =
  let x = inverse a
  in
    putStrLn ("A: \n" ++ matrixToString a) >>
    putStrLn ("inv(A): \n" ++ matrixToString x) >>
    putStrLn ("Verify: A * inv(A)\n" ++ matrixToString (matrixMul a x))

determinantTest :: [[Rational]] -> IO ()
determinantTest a =
  let (l,u,p) = lup a
      det = determinant a
      detL = triangularDet l
      detU = triangularDet u
      exchanges = rowExchanges p
      detPinv = if (exchanges `mod` 2) == 0 then 1 else -1
  in do
    putStrLn ("A: \n" ++ matrixToString a)
    putStrLn ("L: \n" ++ matrixToString l)
    putStrLn ("U: \n" ++ matrixToString u)
    putStrLn ("P: \n" ++ matrixToString p)
    putStrLn "Verify:"
    printf "  Det(L) = %.4f\n" (fromRational detL :: Double)
    printf "  Det(U) = %.4f\n" (fromRational detU :: Double)
    printf "  Det(inv(P)) = %.4f\n" (fromRational detPinv :: Double)
    printf "  Det(A) = %.4f \n" (fromRational det :: Double)



a1 = [[1,2,3], [3,5,6], [1,1,1]] :: [[Rational]]
a2 = [[1,2,1], [3,4,7], [5,7,0]] :: [[Rational]]
a3 = [[11,1,3,2], [9,5,17,5], [24,2,18,7], [2,6,1,1]] :: [[Rational]]
b1 = [1,1,1] :: [Rational]
b2 = [1,2,3,4] :: [Rational]

main = do
  putStrLn "\n-------------     LUP Test     -------------" >> lupTest a1
  putStrLn "\n-------------    Solve Test    -------------" >> solveTest a3 b2
  putStrLn "\n-------------   Inverse Test   -------------" >> inverseTest a2
  putStrLn "\n------------- Determinant Test -------------" >> determinantTest a1



