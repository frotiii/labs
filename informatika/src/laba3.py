def load_data(train_size=0.7):
    """загружает данные Iris и разделяет их на обучающую и тестовую выборки"""
    iris = load_iris()
    X = iris.data[:, :3] 
    y = iris.target
    
    X_train, X_test, y_train, y_test = train_test_split(
        X, y,
        train_size=train_size,
        random_state=42,
        stratify=y
    )
    return X_train, X_test, y_train, y_test

def convert_to_pandas(X, y):
    """конвертирует numpy массивы в pandas структуры"""
    df = pd.DataFrame(X, columns=['sepal_length', 'sepal_width', 'petal_length'])
    target_series = pd.Series(y, name='species')
    return df, target_series

def explore_data(df, target_series):
    """исследует данные, выводя основную информацию"""
    print(df.head())
    print(df.describe())
    print(target_series.value_counts())
    return None

def preprocess_data(X_train, X_test, scaler_type='standard'):
    """применяет указанный метод масштабирования к данным"""
    if scaler_type == 'standard':
        scaler = StandardScaler()
    elif scaler_type == 'minmax':
        scaler = MinMaxScaler()
    elif scaler_type == 'robust':
        scaler = RobustScaler()
    else:
        return None
    
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    return X_train_scaled, X_test_scaled

def train_model(X_train, y_train, max_depth=3):
    """
    обучает модель
    max_depth - максимальная глубина дерева;
    меньшие значения уменьшают переобучение, но могут недообучать
    большие значения повышают точность, но могут переобучать
    """
    model = RandomForestClassifier(max_depth=max_depth, random_state=42)
    model.fit(X_train, y_train)
    return model

def predict(model, X_test):
    """выполняет предсказание на тестовых данных"""
    return model.predict(X_test)

def evaluate(y_pred, y_test):
    """
    оценивает точность модели
    на точность влияют:
    качество предобработки данных
    параметры модели (max_depth)
    баланс классов в данных
    """
    accuracy = accuracy_score(y_test, y_pred)
    return round(accuracy, 3)