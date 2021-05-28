# Ch. 1 Strategy Pattern Homework

'+', '-', '*', '/' 4가지 연산에 대해서 서로 다른 방식으로 구현한다.
- [x] dirty code
- [x] strategy pattern
- [x] template method pattern
- [x] lambda experssion

# SOLID 원칙 (객체지향 5대 원칙)

1. Single Responsibility Principle 단일 책임 원칙
   - 하나의 클래스는 단 한가지의 변경 이유만을 가져야한다.
   - 하나의 모듈은 하나의, 오직 하나의 액터에 대해서만 책임져야 한다.
2. Open-Closed Principle 개방 패쇄 원칙
   - 소프트웨어 개체는 확장에는 열려 있어야 하고, 변경에는 닫혀 있어야 한다.
   - 시스템을 확장하기 쉬운 동시에 변경으로 인해 시스템이 너무 많은 영향을 받지 않도록 하는데 있다.
3. Liskov Substitution Priciple 리스코프 치환 원칙
4. Interface Segregation Priciple 인터페이스 분리 원칙
   - 자신이 사용하지 않는 메서드에 의존하지 않는다.
   - 어떤 클라이언트는 사용하고, 다른 클라이언트는 사용하지 않는 메소드가 있을 때, 이 메소드의 변경이 사용하지 않는 클라이언트에게 영향을 준다면 인터페이스를 분리하도록 하자.
5. Dependency Inversion Principle 의존성 역전 원칙
   - 변동성이 큰 구체 클래스를 참조하지 말자.
   - 변동성이 큰 구체 클래스로부터 파생하지 말자.
   - 구체 함수를 오버라이드 하지 말자.
   - 구체적이며 변동성이 크다면 절대로 그 이름을 언급하지 말자.