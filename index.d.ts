export interface Point { x: number; y: number }

export type Fabric =
  | { width: number; length: number }
  | { boundary: Point[] };

export interface VariantConstraint {
  rotationAngle?: number;
  isMirrored?: boolean;
  requiredQuantity?: number;
  twoWay?: boolean;
}

export interface PatternPiece {
  id: number;
  points: Point[];
  constraints?: VariantConstraint[];
  seamAllowance?: number;
  cuttingAllowance?: number;
}

export interface ItemPlacement {
  id: number;
  rotationAngle: number;
  isMirrored: boolean;
  isFlipped180: boolean;
  x: number;
  y: number;
}

export interface NestingStats {
  usableScrapCount: number;
  usableScrapsArea: number;
  usableScrapsPercentage: number;
  remainingFabricLength: number;
  remainingFabricArea: number;
  remainingFabricPercentage: number;
  wasteArea: number;
  wastePercentage: number;
}

export interface NestingResult {
  wasSuccessful: boolean;
  minLength: number;
  executionTime: number;
  stats: NestingStats;
  positions: ItemPlacement[];
}

export function execute(fabric: Fabric, pieces: PatternPiece[]): Promise<NestingResult>;
export function executeSync(fabric: Fabric, pieces: PatternPiece[]): NestingResult;
